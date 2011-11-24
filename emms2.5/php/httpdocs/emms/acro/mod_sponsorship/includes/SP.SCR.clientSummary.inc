<?php

$show_gohome = true;

$data = WEBPAGE::$dbh->getAll(sprintf("
  select
    z.id,
    z.zone,
    count(c.id) qty,
    c.gender,
    round(AVG(YEAR(NOW())-YEAR(c.birthdate))) age,
    round(avg(c.dependants),1) dep
  from
    tblClients c,
    tblLoans l,
    tblZones z,
    tblLoansMasterDetails lmd,
    (
      select
        lms.master_id
      from
        tblLoansMasterSponsors lms,
        tblLoansMaster lm
      where
        lms.sponsor_id = %s and
        lm.id = lms.master_id and
        lm.check_status = 'R'
      group by
        lms.master_id
    ) lmx
  where
    c.id               = l.client_id   and
    z.id               = l.zone_id     and
    l.id               = lmd.loan_id   and
    lmd.master_id      = lmx.master_id and
    YEAR(c.birthdate) < YEAR(NOW())-10 and
    YEAR(NOW())-YEAR(c.birthdate) < 90
  group by
    l.zone_id,c.gender",WEBPAGE::$userID));

foreach($data as $key=>$val)
{
  $mrow[$val['id']]['zone']  = '';
  $mrow[$val['id']]['qty_F'] = '';
  $mrow[$val['id']]['qty_M'] = '';
  $mrow[$val['id']]['age_F'] = '';
  $mrow[$val['id']]['age_M'] = '';
  $mrow[$val['id']]['dep_F'] = '';
  $mrow[$val['id']]['dep_M'] = '';

  $total = max($total,$val['id']);
}

$total++;

$mrow[$total]['zone']  = 'total';
$mrow[$total]['qty_F'] = '';
$mrow[$total]['qty_M'] = '';
$mrow[$total]['age_F'] = '';
$mrow[$total]['age_M'] = '';
$mrow[$total]['dep_F'] = '';
$mrow[$total]['dep_M'] = '';

foreach($data as $key=>$val)
{
  $mrow[$val['id']]['zone']                           = $val['zone'];
  $mrow[$val['id']][sprintf('qty_%s',$val['gender'])] = number_format($val['qty'],0);
  $mrow[$val['id']][sprintf('age_%s',$val['gender'])] = $val['age'];
  $mrow[$val['id']][sprintf('dep_%s',$val['gender'])] = $val['dep'];

  $mrow[$total][sprintf('qty_%s',$val['gender'])]   += $val['qty'];
  $mrow[$total][sprintf('age_%s',$val['gender'])]   += $val['qty']*$val['age'];
  $mrow[$total][sprintf('dep_%s',$val['gender'])]   += $val['qty']*$val['dep'];
}

$mrow[$total]['age_F'] = round($mrow[$total]['age_F']/$mrow[$total]['qty_F'],0);
$mrow[$total]['age_M'] = round($mrow[$total]['age_M']/$mrow[$total]['qty_M'],0);
$mrow[$total]['dep_F'] = round($mrow[$total]['dep_F']/$mrow[$total]['qty_F'],1);
$mrow[$total]['dep_M'] = round($mrow[$total]['dep_M']/$mrow[$total]['qty_M'],1);
$mrow[$total]['qty_F'] = number_format($mrow[$total]['qty_F'],0);
$mrow[$total]['qty_M'] = number_format($mrow[$total]['qty_M'],0);

$extra['zone']['header']   = 'tblZones.zone';
$extra['zone']['pack']     = '%s';
$extra['zone']['align']    = 'left';

$extra['qty_F']['header']  = 'qty_F';
$extra['qty_F']['pack']    = '%s';
$extra['qty_F']['align']   = 'right';

$extra['qty_M']['header']  = 'qty_M';
$extra['qty_M']['pack']    = '%s';
$extra['qty_M']['align']   = 'right';

$extra['age_F']['header']  = 'age_F';
$extra['age_F']['pack']	   = '%s';
$extra['age_F']['align']   = 'right';

$extra['age_M']['header']  = 'age_M';
$extra['age_M']['pack']	   = '%s';
$extra['age_M']['align']   = 'right';

$extra['dep_F']['header']  = 'dep_F';
$extra['dep_F']['pack']	   = '%s';
$extra['dep_F']['align']   = 'right';

$extra['dep_M']['header']  = 'dep_M';
$extra['dep_M']['pack']	   = '%s';
$extra['dep_M']['align']   = 'right';

$extra['param']['ref']     = WEBPAGE::$scr_name;
$extra['param']['ts']      = microtime(true);

array_unshift($mrow,$extra);

WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache.'.'.$extra['param']['ts']);
require_once './includes/BS.SCR.pager.inc';

?>

