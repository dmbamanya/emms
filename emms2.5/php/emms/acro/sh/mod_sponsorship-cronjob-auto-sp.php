#!/usr/bin/php
<?php

$path = '/var/www/vhosts/esperanza.org/emms/debug';
$conf = parse_ini_file($path . '/conf.ini', true);

$linkID = @mysql_connect($conf['db']['host'],$conf['db']['usr'],$conf['db']['pwd']) or die("Could not connect to MySQL server");

@mysql_select_db($conf['db']['name']) or die("Could not select db");

$lm = mysql_query(sprintf("
  select distinct 
    lm.id, 1 + %s - datediff(curdate(),ckd.date) time_left, 
    coalesce(rc.donations,0.00) donations, 
    lm.amount - coalesce(rc.donations,0.00) pending,
    z.id zone
  from 
  (
    tblLoansMaster lm, 
    tblZones z,
    (
      select 
        lmd.master_id, 
	lsh.date 
      from 
        tblLoanStatusHistory lsh, 
	tblLoansMasterDetails lmd 
      where 
        lmd.loan_id = lsh.loan_id and 
	lsh.status = 'G' and 
	lsh.date = date_add(curdate(), interval -(%s + 1) day)
    ) ckd 
  ) 
  left join 
  (
    select 
      master_id, 
      sum(donation) donations 
    from 
      tblLoansMasterSponsors 
    group by 
      master_id
  ) rc on rc.master_id = lm.id 
  where 
    lm.borrower_type = 'B' and 
    lm.check_status = 'R' and 
    z.parent_id = lm.zone_id and
    z.program_id = lm.program_id and
    ckd.master_id = lm.id and 
    coalesce(rc.donations,0.00) < lm.amount 
  order by 
    lm.id",5,5));

$sd = mysql_query(sprintf("
  select
    sd.sponsor_id, 
    sum(sd.conv_amount) donations
  from
  (
    tblSponsorsDonations sd,
    tblSponsors s
  )
  where
    s.id = sd.sponsor_id
  group by
    s.id"));

$sp = mysql_query(sprintf("
  select
    lms.sponsor_id,
    sum(if(lcd.balance_kp,lcd.balance_kp*(lms.donation/lm.amount),0)) balance,
    sum(if(lwo.principal,lwo.principal*(lms.donation/lm.amount),0)) write_off
  from
  (
    tblLoansMasterSponsors lms,
    tblLoansMaster lm,
    tblLoansMasterDetails lmd,
    tblLoans l
  )
  left join
    tblLoansCurrentData lcd on lcd.loan_id = lmd.loan_id
  left join
    tblLoanWriteOff lwo on lwo.loan_id = lmd.loan_id
  where
    lm.id = lms.master_id and
    lm.check_status = 'R' and
    lmd.master_id = lms.master_id and
    l.id = lmd.loan_id
  group by
    lms.sponsor_id"));
    
while($row = mysql_fetch_array($sd))
{
  $s[$row['sponsor_id']]['donations'] = $row['donations'];
  $s[$row['sponsor_id']]['balance'] = 0;
  $s[$row['sponsor_id']]['write_off'] = 0;
  $s[$row['sponsor_id']]['funds'] = $row['donations'];
  $s[$row['sponsor_id']]['priority'] = 1;
}

while($row = mysql_fetch_array($sp))
{
  $s[$row['sponsor_id']]['balance'] = $row['balance'];
  $s[$row['sponsor_id']]['write_off'] = $row['write_off'];
  $s[$row['sponsor_id']]['funds'] = $s[$row['sponsor_id']]['donations'] - $row['balance'] - $row['write_off'];
  $s[$row['sponsor_id']]['priority'] = 1 - ($s[$row['sponsor_id']]['balance']/($s[$row['sponsor_id']]['donations']-$s[$row['sponsor_id']]['write_off']));
}

while($row = mysql_fetch_array($lm))
{
  while(($row['pending'] > 0)&&($pid = getMaxPriorityId($s)))
  {
    $amt = min($row['pending'],$s[$pid]['funds']);
    $row['pending'] = $row['pending'] - $amt;
    $s[$pid]['balance'] = $s[$pid]['balance'] + $amt;
    $s[$pid]['funds'] = $s[$pid]['funds'] - $amt;
    $s[$pid]['priority'] = 1 - ($s[$pid]['balance']/($s[$pid]['donations']-$s[$pid]['write_off']));
    mysql_query(sprintf("insert into tblLoansMasterSponsors (master_id,sponsor_id,donation,tip,datetime) values ('%s','%s','%s','%s','%s')",$row['id'],$pid,$amt,0,gmdate("Y-m-d H:i:s", time())));
  }
}

function getMaxPriorityId($s)
{
  $pid = key($s);
  $pval = $s[$pid]['priority'];
  foreach($s as $id => $data)
  {
    if ($data['priority'] > $pval) 
    { 
      $pid = $id;
      $pval = $data['priority'];
    }
  }
  return $pid;
}

mysql_close();

?>