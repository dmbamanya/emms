<?php
error_reporting(E_ERROR);
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once './includes/ST.LIB.login.inc';
WEBPAGE::START();
WEBPAGE::$lang = isset($_GET['lang']) ? $_GET['lang'] : WEBPAGE::$conf['app']['default_language'];
WEBPAGE::load_gt();

/*
 *  Exit the script is WEBPAGE::$runMode has an unexpected value.
 */
switch (WEBPAGE::$runMode)
{
  case WEBPAGE::_RUN_MODE_OUTDATED:
    break;
  default :
    if (WEBPAGE::$conf['app']['auto_cron'] == '1') { exit; }
    WEBPAGE::redirect(sprintf('index.php?lang=%s',WEBPAGE::$lang));
    exit;
}

/*
 *  Set $p_date. $p_date must be set to 1 day after WEBPAGE::$runDate, which is the previous script run date.
 */
$rdate = explode('-',WEBPAGE::$runDate);
$p_date = date('Y-m-d',mktime(0,0,0,$rdate[1],$rdate[2],$rdate[0],0) + 24*60*60);

/*
 *  Set the system in 'maintenance' mode. User login is disable, current sessions are closed.
 */
WEBPAGE::$dbh->query(sprintf("insert into tblDataLog values ('null','%s',CURTIME(),'maintenance','index.cron.php','','')",$p_date));

/*
 *  Run process
 */

/*
 *  Deactivate clients that remains for more than WEBPAGE::$conf['app']['auto_deactivate_client_margin'] days
 *  without an active loan or a loan request.
 */
if (WEBPAGE::$conf['app']['auto_deactivate_client_max'])
{
  $cdata = WEBPAGE::$dbh->getAll(sprintf("
    select
      bl.id
    from
      (
        select
          c.id, c.society_id, c.advisor_id, c.zone_id
        from
          tblClients c
        left join tblLoans n  on  n.client_id = c.id and  n.status = 'N'
        left join tblLoans o  on  o.client_id = c.id and  o.status = 'O'
        left join tblLoans ro on ro.client_id = c.id and ro.status = 'RO'
        left join tblLoans r  on  r.client_id = c.id and  r.status = 'R'
        left join tblLoans s  on  s.client_id = c.id and  s.status = 'S'
        left join tblLoans a  on  a.client_id = c.id and  a.status = 'A'
        left join tblLoans d  on  d.client_id = c.id and  d.status = 'D'
        left join tblLoans g  on  g.client_id = c.id and  g.status = 'G'
        left join tblLoans li on li.client_id = c.id and li.status = 'LI'
        where
          c.advisor_id != 0 and
          n.id  is null     and
          o.id  is null     and
          ro.id is null     and
          r.id  is null     and
          s.id  is null     and
          a.id  is null     and
          d.id  is null     and
          g.id  is null     and
          li.id is null
      ) bl
    left join
      (
        select
          l.client_id
        from
          tblLoans l, tblLoanStatusHistory lsh, view_loan_status_max_id lsid
        where
          lsid.loan_id  = l.id    and
          lsh.id        = lsid.id and
          lsh.date     >= date_sub('%s', interval %s day)
      ) wl on wl.client_id = bl.id
    where
      wl.client_id is null
    limit %s",
    $p_date,
    WEBPAGE::$conf['app']['auto_deactivate_client_margin'],
    WEBPAGE::$conf['app']['auto_deactivate_client_max']));
  foreach($cdata as $key=>$val)
  {
     /*
      *  Log client deactivation, then submit deactivation
      */
     WEBPAGE::$dbh->query(sprintf('
       insert into
         tblClientIOM
           (type,client_id,society_id,advisor_id,zone_id,date,cause,user_id,memo)
         select
           "O",c.id,c.society_id,c.advisor_id,c.zone_id,"%s","%s",1,"%s"
         from
           tblClients c
         where
           c.id = "%s"',
       $p_date,
       WEBPAGE::$conf['app']['auto_deactivate_client_cause'],
       WEBPAGE::$conf['app']['auto_deactivate_client_memo'],
       $val['id']));
     /*
      * Submit deactivation
      */
     WEBPAGE::$dbh->query(sprintf('
       update
         tblClients
       set
         zone_id = 0,
         advisor_id = 0,
         society_id = 0,
         editor_id = 1,
         editor_date = "%s"
       where
         id = %s',
       $p_date,
       $val['id']));
  }
}

/*
 *  Update tblClientPortfolio.
 *  tblClientPortfolio holds snapshots of the client portfolio.
 *  One new snapshot every day, for each branch, program and loan officer.
 */
$cdata = WEBPAGE::$dbh->getAll(sprintf("
           select
             count(c.id) hits,
             z.parent_id,
             z.program_id,
             c.advisor_id,
             c.gender,
             if(s.category,s.category,'I') category
           from
             (tblClients c, tblZones z)
           left join
             tblSocieties as s on s.id = c.society_id
           where
             z.id = c.zone_id and
             c.advisor_id != 0
           group by
             z.parent_id,
             z.program_id,
             c.advisor_id,
             c.gender,
             category"));
$sdata = WEBPAGE::$dbh->getAll(sprintf("
           select
             count(s.id) hits,
             z.parent_id,
             z.program_id,
             s.advisor_id,
             s.category
           from
             tblSocieties as s,
             tblZones as z
           where
             z.id = s.zone_id and
             s.advisor_id != 0
           group by
             z.parent_id,
             z.program_id,
             s.advisor_id,
             s.category"));
$acdata = WEBPAGE::$dbh->getAll(sprintf("
            select
              count(c.id) hits,
              z.parent_id,
              z.program_id,
              c.advisor_id
            from
              tblClients c,
              tblZones z
            where
              c.zone_id = z.id and
              c.advisor_id != 0 and
              c.id in ( select l.client_id from tblLoans l, tblLoansCurrentData lcd where l.id = lcd.loan_id )
            group by
              z.parent_id,
              z.program_id,
              c.advisor_id;"));
$token = array();
$c_data = array();
foreach ($cdata as $key=>$val)
{
  $c_data[$val['parent_id']][$val['program_id']][$val['advisor_id']]['clients']        += $val['hits'];
  $c_data[$val['parent_id']][$val['program_id']][$val['advisor_id']][$val['gender']]   += $val['hits'];
  $c_data[$val['parent_id']][$val['program_id']][$val['advisor_id']][$val['category']] += $val['hits'];
  $token[sprintf('%s.%s.%s',$val['parent_id'],$val['program_id'],$val['advisor_id'])]   = 1;
}
$s_data = array();
foreach ($sdata as $key=>$val)
{
  $s_data[$val['parent_id']][$val['program_id']][$val['advisor_id']][$val['category']] += $val['hits'];
  $token[sprintf('%s.%s.%s',$val['parent_id'],$val['program_id'],$val['advisor_id'])]   = 1;
}
$ac_data = array();
foreach ($acdata as $key=>$val)
{
  $ac_data[$val['parent_id']][$val['program_id']][$val['advisor_id']] ['client_al']   += $val['hits'];
  $token[sprintf('%s.%s.%s',$val['parent_id'],$val['program_id'],$val['advisor_id'])]  = 1;
}
$inx = array();
foreach($token as $key=>$val)
{
  $inx = explode('.',$key);
  WEBPAGE::$dbh->query(sprintf("
    insert into
      tblClientPortfolio (date,zone_id,program_id,advisor_id,clients,female,male,client_i,client_g,client_b,group_g,group_b,group_bg,client_al)
    values
      ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
    $p_date,
    $inx[0],
    $inx[1],
    $inx[2],
    $c_data[$inx[0]][$inx[1]][$inx[2]]['clients'],
    $c_data[$inx[0]][$inx[1]][$inx[2]]['F'],
    $c_data[$inx[0]][$inx[1]][$inx[2]]['M'],
    $c_data[$inx[0]][$inx[1]][$inx[2]]['I'],
    $c_data[$inx[0]][$inx[1]][$inx[2]]['G'],
    $c_data[$inx[0]][$inx[1]][$inx[2]]['B'],
    $s_data[$inx[0]][$inx[1]][$inx[2]]['G'],
    $s_data[$inx[0]][$inx[1]][$inx[2]]['B'],
    $s_data[$inx[0]][$inx[1]][$inx[2]]['BG'],
    $ac_data[$inx[0]][$inx[1]][$inx[2]]['client_al']));
}

/*
 *  Update tblLoansCurrentData with interest, delay and penalties acumulated on each active loan.
 *  Note that tblLoansCurrentData only deals with the next expected payment for each active loan,
 *  for loans with more than 1 due payment, only the older due payment values are considered.
 */
WEBPAGE::$dbh->query(sprintf("
  update
    tblLoansCurrentData lcd,
    tblLoans l,
    tblLoanTypes lt
  set
    lcd.cn_date = '%s',
    lcd.cn_delay = GREATEST(0,(TO_DAYS('%s')-TO_DAYS(xp_pmt_date))),
    lcd.cn_penalties = IF(lcd.cn_delay<=l.margin_d,0,0.01*l.rates_d*GREATEST(0,(TO_DAYS('%s')-TO_DAYS(xp_pmt_date)))*lcd.xp_pmt)
  where
    l.id = lcd.loan_id and
    lt.id = l.loan_type_id",$p_date,$p_date,$p_date));

/*
 *  Update tblRiskPortfolio
 *  tblRiskPortfolio holds snapshots of the portfolio risk.
 *  One new snapshot every day, for each branch, program and loan officer.
 */
$dataA = array();
$dataB = array();
$dataC = array();
$dataD = array();
$riskA = WEBPAGE::$dbh->getAll(sprintf("
           select
             sum(lcd.balance_kp) kp,
             l.zone_id,
             l.program_id,
             advisor_id
           from
             tblLoansCurrentData lcd,
             tblLoans l
           where
             l.id = lcd.loan_id and
             lcd.cn_delay >= '%s'
           group by
             l.zone_id,
             l.program_id,
             l.program_id,
             l.advisor_id",
           WEBPAGE::$conf['app']['risk.days.A']));
foreach($riskA as $key=>$val)
{ 
  $dataA[$val['zone_id']][$val['program_id']][$val['advisor_id']] = $val['kp'];
}
$riskB = WEBPAGE::$dbh->getAll(sprintf("
           select
             sum(lcd.balance_kp) kp,
             l.zone_id,
             l.program_id,
             advisor_id
           from
             tblLoansCurrentData lcd,
             tblLoans l
           where
             l.id = lcd.loan_id and
             lcd.cn_delay >= '%s'
           group by
             l.zone_id,
             l.program_id,
             l.program_id,
             l.advisor_id",
           WEBPAGE::$conf['app']['risk.days.B']));
foreach($riskB as $key=>$val)
{ 
  $dataB[$val['zone_id']][$val['program_id']][$val['advisor_id']] = $val['kp'];
}
$riskC = WEBPAGE::$dbh->getAll(sprintf("
           select
             sum(lcd.balance_kp) kp,
             l.zone_id,
             l.program_id,
             advisor_id
           from
             tblLoansCurrentData lcd,
             tblLoans l
           where
             l.id = lcd.loan_id and
             lcd.cn_delay >= '%s'
           group by
             l.zone_id,
             l.program_id,
             l.program_id,
             l.advisor_id",
           WEBPAGE::$conf['app']['risk.days.C']));
foreach($riskC as $key=>$val)
{ 
  $dataC[$val['zone_id']][$val['program_id']][$val['advisor_id']] = $val['kp'];
}
$riskD = WEBPAGE::$dbh->getAll(sprintf("
           select
             sum(lcd.balance_kp) kp,
             l.zone_id,
             l.program_id,
             advisor_id
           from
             tblLoansCurrentData lcd,
             tblLoans l
           where
             l.id = lcd.loan_id and
             lcd.cn_delay >= '%s'
           group by
             l.zone_id,
             l.program_id,
             l.program_id,
             l.advisor_id",
           WEBPAGE::$conf['app']['risk.days.D']));
foreach($riskD as $key=>$val)
{ 
  $dataD[$val['zone_id']][$val['program_id']][$val['advisor_id']] = $val['kp'];
}
$balance = WEBPAGE::$dbh->getAll(sprintf("
             select
               sum(lcd.balance_kp) kp,
               l.zone_id,
               l.program_id,
               advisor_id
             from
               tblLoansCurrentData lcd,
               tblLoans l
             where
               l.id = lcd.loan_id
             group by
               l.zone_id,
               l.program_id,
               l.program_id,
               l.advisor_id"));
$c = 0;
$data  = array();
foreach($balance as $key=>$val) 
{
  $data['riskA'] = max(0,$dataA[$val['zone_id']][$val['program_id']][$val['advisor_id']]);
  $data['riskB'] = max(0,$dataB[$val['zone_id']][$val['program_id']][$val['advisor_id']]);
  $data['riskC'] = max(0,$dataC[$val['zone_id']][$val['program_id']][$val['advisor_id']]);
  $data['riskD'] = max(0,$dataD[$val['zone_id']][$val['program_id']][$val['advisor_id']]);
  WEBPAGE::$dbh->query(sprintf("
    insert into
      tblRiskPortfolio (date,zone_id,program_id,advisor_id,balance,riskA,riskB,riskC,riskD)
    values
      ('%s','%s','%s','%s','%s','%s','%s','%s','%s')",
    $p_date,
    $val['zone_id'],
    $val['program_id'],
    $val['advisor_id'],
    $val['kp'],
    $data['riskA'],
    $data['riskB'],
    $data['riskC'],
    $data['riskD']));
}

/*
 *  Update tblLoansOnDelinquency
 *  One new record reflecting several stats is created every day for each delinquent loan.
 *  Depending on the delinquent portfolio size, this update could take quite some time,
 *  be sure to adjust the php run overtime accordingly.
 */
$dloans = WEBPAGE::$dbh->getAll("select loan_id from tblLoansCurrentData where cn_delay > 0");
require_once 'Date.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
$c = 0;
$ldata = array();
foreach ($dloans as $key=>$val)
{ 
  $loan = new LOAN($val['loan_id']);
  $balance_kp = $loan->data['balance_kp'];
  $hits = 0;
  while(($loan->data['xp_pmt_date'] <= $loan->data['cn_date'])&&($balance_kp > 0))
  {
    $data                       = $loan->getNextPaymentData();
    $loan->data['xp_pmt_date']  = $data['xp_pmt_date'];
    $loan->data['xp_pmt']       = $data['xp_pmt'];
    $loan->data['balance_kp']   = $data['balance_kp'];
    $loan->data['balance_kaf']  = $data['balance_kaf'];
    $loan->data['balance_kat']  = $data['balance_kat'];
    $loan->data['r_from_date']  = $data['r_from_date'];
    $ldata[$c]['fees']         += $data['fees'];
    $ldata[$c]['insurances']   += $data['insurances'];
    $ldata[$c]['principal']    += $data['principal'];
    $ldata[$c]['interest']     += $data['interest'];
    $ldata[$c]['delay']        += $data['delay'];
    $ldata[$c]['penalties']    += $data['penalties'];
    $ldata[$c]['pmt']          += $data['pmt'];
    $balance_kp                 = $data['balance_kp'];
    $hits++;
    }
  $ldata[$c]['loan_id'] = $val['loan_id'];
  $ldata[$c]['date']    = $loan->data['cn_date'];
  $ldata[$c]['hits']    = $hits;
  $c++;
}
foreach($ldata as $key=>$val)
{
  WEBPAGE::$dbh->query(sprintf("
    insert into
      tblLoansOnDelinquency
    values
      ('null','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
    $val['loan_id'],
    $val['date'],
    $val['hits'],
    $val['delay'],
    $val['pmt'],
    $val['penalties'],
    $val['interest'],
    $val['fees'],
    $val['insurances'],
    $val['principal']));
}

/*
 *  Update tblTCredits
 *  A new record is created every day for each branch, program and fund.
 *  Each record has a transaction code that represents all credit transactions
 *  completed in one specific date for each specific branch, program and fund.
 */
WEBPAGE::$dbh->query("
  insert into
    tblTCredits
      (code,date,branch_id,program_id,fund_id,amount,principal,fees,insurances,interest,penalties)
    select
      CONCAT('C',p.date+0,LPAD(l.zone_id,3,'0'),LPAD(l.program_id,3,'0'),LPAD(flmp.fund_id,3,'0')) transaction,
      p.date,
      l.zone_id,
      l.program_id,
      flmp.fund_id,
      sum(p.pmt),
      sum(p.principal),
      sum(p.fees),
      sum(p.insurances),
      sum(p.interest),
      sum(p.penalties)
    from
      tblPayments p,
      tblLoans l,
      tblFundsLoansMasterPct flmp,
      tblLoansMasterDetails lmd
    where
      lmd.loan_id = l.id and
      flmp.master_id = lmd.master_id and
      p.transaction_id = '0' and
      l.id = p.loan_id
    group by
      transaction");

/*
 *  Update tblPayments
 *  Set the credit transaction code for each payment. The transaction code represents
 *  all credit transactions completed in one specific date for each specific branch, program and fund.
 *  That is, all payments registered in the same date, branch, program and fund
 *  will share the same transaction code.
 */
WEBPAGE::$dbh->query("
  update
    tblPayments p,
    tblLoans l,
    tblFundsLoansMasterPct flmp,
    tblLoansMasterDetails lmd
  set
    p.transaction_id = CONCAT('C',p.date+0,LPAD(l.zone_id,3,'0'),LPAD(l.program_id,3,'0'),LPAD(flmp.fund_id,3,'0'))
  where
    p.transaction_id = 0 and
    l.id = p.loan_id and
    lmd.loan_id = l.id and
    flmp.master_id = lmd.master_id");

/*
 *  tblTDebits is always up to date - from LN.SCR.doCheckRelease.inc
 */

/*
 *  Refresh tblLoansCurrentDataBackup
 *  tblLoansCurrentDataBackup is used to rollback changes in tblLoansCurrentData
 *  if a payment is registered by mistake.
 *  Note: Only payments without a transaction code can be rolled back.
 */
WEBPAGE::$dbh->query("delete from tblLoansCurrentDataBackup");
WEBPAGE::$dbh->query("insert into tblLoansCurrentDataBackup select * from tblLoansCurrentData");

/*
 *  If WEBPAGE::$conf['app']['auto_write_off'] is set,
 *  write-off all loans with more than WEBPAGE::$conf['app']['auto_write_off_margin'] days
 *  behind schedule
 */
if (WEBPAGE::$conf['app']['auto_write_off'])
{
  $lod_id = WEBPAGE::$dbh->getAll(sprintf("
              select
                max(lod.id) id,
                lcd.loan_id,
                lcd.balance_kp,
                lcd.balance_kaf,
                lcd.balance_kat,
                lcd.cn_date
              from
                tblLoansOnDelinquency lod,
                tblLoansCurrentData lcd
              where
                lcd.loan_id = lod.loan_id and
                lcd.cn_delay > '%s'
              group by
                lod.loan_id",
              WEBPAGE::$conf['app']['auto_write_off_margin']));
  foreach($lod_id as $key=>$val)
  {
    $lod_info = current(WEBPAGE::$dbh->getAll(sprintf("select lod.interest,lod.penalties from tblLoansOnDelinquency lod where lod.id = '%s'",$val['id'])));
    $amount = $val['balance_kp'] + $val['balance_kaf'] + $val['balance_kat'] + $lod_info['interest'] + $lod_info['penalties'];
    WEBPAGE::$dbh->query(sprintf("
      insert into
        tblLoanWriteOff (id,loan_id,amount,principal,insurance,fees,interest,penalties,date,user_id)
      values
        ('Null','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
      $val['loan_id'],
      $amount,
      $val['balance_kp'],
      $val['balance_kaf'],
      $val['balance_kat'],
      $lod_info['interest'],
      $lod_info['penalties'],
      $val['cn_date'],
      '1'));
    WEBPAGE::$dbh->query(sprintf("delete from tblLoansCurrentData where loan_id = %s", $val['loan_id']));
    WEBPAGE::$dbh->query(sprintf("
      insert into
        tblLoanStatusHistory (id,loan_id,p_status,status,date,user_id,memo)
      values
        ('Null','%s','G','LI','%s','1','%s')",
      $val['loan_id'],
      $val['cn_date'],
      WEBPAGE::$gt['loanWriteOff']));
    WEBPAGE::$dbh->query(sprintf("
      update
        tblLoans
      set
        status = 'LI',
        editor_id = '1',
        editor_date = '%s'
      where
        id = '%s'",
      $val['cn_date'],$val['loan_id']));
  }
}

/*
 *  Set the system in 'normal' mode.
 *  If $p_date is equal to the current server date, this enables back user's login.
 */
WEBPAGE::$dbh->query(sprintf("insert into tblDataLog values ('null','%s',CURTIME(),'normal','index.cron.php','','')",$p_date));

if (WEBPAGE::$conf['app']['auto_cron'] != '1') { WEBPAGE::redirect('index.cron.php'); exit; }

?>