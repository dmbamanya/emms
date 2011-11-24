<?php
$show_gohome = true;

$donation = WEBPAGE::$dbh->getAll(sprintf("
select
  count(lmx.master_id) loans,
  sum(lmx.donation) donation,
  sum(lmx.amount) kp,
  z.zone branch
from
  (
    select
      lms.master_id,
      sum(lms.donation) donation,
      lm.zone_id,
      lm.amount
    from
      tblLoansMasterSponsors lms,
      tblLoansMaster lm
    where
      lms.sponsor_id = %s and
      lm.id = lms.master_id and
      lm.check_status = 'R'
    group by
      lms.master_id
  ) lmx,
  tblZones z
where
  z.id = lmx.zone_id
group by
  lmx.zone_id", WEBPAGE::$userID));

$repayment = WEBPAGE::$dbh->getAssoc(sprintf("
select
  z.zone branch,
  sum(p.principal) repayment
from
  tblPayments p,
  tblLoansMasterDetails lmd,
  tblLoansMaster lm,
  tblZones z,
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
  lmd.loan_id = p.loan_id and
  lmx.master_id = lmd.master_id and
  lm.id = lmd.master_id and
  z.id = lm.zone_id
group by
  lm.zone_id", WEBPAGE::$userID));

$balance = WEBPAGE::$dbh->getAssoc(sprintf("
select
  z.zone branch,
  round(sum(lcd.balance_kp*(lmx.donation/lmx.amount)),2)
from
  tblLoansCurrentData lcd,
  tblLoansMasterDetails lmd,
  tblLoansMaster lm,
  tblZones z,
  (
      select
        lms.master_id,
        sum(lms.donation) donation,
        lm.amount
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
  lmd.loan_id = lcd.loan_id and
  lmx.master_id = lmd.master_id and
  lm.id = lmd.master_id and
  z.id = lm.zone_id
group by
  z.id", WEBPAGE::$userID));

$write_off = WEBPAGE::$dbh->getAssoc(sprintf("
select
  z.zone branch,
  sum(lwo.principal) write_off
from
  tblLoanWriteOff lwo,
  tblLoansMasterDetails lmd,
  tblLoansMaster lm,
  tblZones z,
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
  lmd.loan_id = lwo.loan_id and
  lmx.master_id = lmd.master_id and
  lm.id = lmd.master_id and
  z.id = lm.zone_id
group by
  lm.zone_id", WEBPAGE::$userID));

$total = count($donation) + 1;
$mrow[$total]['branch'] = 'total';

foreach($donation as $key => $val)
{
  $mrow[$total]['loans']       += $val['loans'];
  $mrow[$total]['donation']    += $val['donation'];
  $mrow[$total]['repayment']   += $repayment[$val['branch']];
  $mrow[$total]['write_off']   += $write_off[$val['branch']];
  $mrow[$total]['balance']     += $balance[$val['branch']];

  $mrow[$key]['branch']          = $val['branch'];
  $mrow[$key]['loans']           = number_format($val['loans'],0);
  $mrow[$key]['donation']        = number_format($val['donation'],2);
  $mrow[$key]['repayment']       = number_format($repayment[$val['branch']],2);
  $mrow[$key]['write_off']       = number_format($write_off[$val['branch']],2);
  $mrow[$key]['balance']         = number_format($balance[$val['branch']],2);

}

$mrow[$total]['loans']       = number_format($mrow[$total]['loans'],0);
$mrow[$total]['donation']    = number_format($mrow[$total]['donation'],2);
$mrow[$total]['repayment']   = number_format($mrow[$total]['repayment'],2);
$mrow[$total]['write_off']   = number_format($mrow[$total]['write_off'],2);
$mrow[$total]['balance']     = number_format($mrow[$total]['balance'],2);

$mrow = array_reverse($mrow);

$extra['branch']['header']    = 'tblZones.zone';
$extra['branch']['pack']      = '%s';
$extra['branch']['align']     = 'right';

$extra['loans']['header']     = 'loans';
$extra['loans']['pack']       = '%s';
$extra['loans']['align']      = 'right';

$extra['donation']['header']  = 'amount_disbursed';
$extra['donation']['pack']    = '%s';
$extra['donation']['align']   = 'right';

$extra['repayment']['header'] = 'principalRepayment';
$extra['repayment']['pack']   = '%s';
$extra['repayment']['align']  = 'right';

$extra['write_off']['header'] = 'writeOff_amount';
$extra['write_off']['pack']   = '%s';
$extra['write_off']['align']  = 'right';

$extra['balance']['header']   = 'portfolio_balance';
$extra['balance']['pack']     = '%s';
$extra['balance']['align']    = 'right';

$extra['param']['ref']        = WEBPAGE::$scr_name;
$extra['param']['ts']         = microtime(true);

array_unshift($mrow,$extra); 

WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache.'.'.$extra['param']['ts']);
require_once './includes/BS.SCR.pager.inc';


?>