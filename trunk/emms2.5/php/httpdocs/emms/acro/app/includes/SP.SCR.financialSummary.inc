<?php

$sponsor = WEBPAGE::$dbh->getAll(sprintf("select sp.sponsor from tblSponsors sp where sp.id =%s",$_REQUEST['id']));

$data = WEBPAGE::$dbh->getAll(sprintf("
         select
 x1.*, x2.*
from
(
select
  count(distinct lmd.loan_id) borrowers,
  count(distinct lms.master_id) loans,
  round(sum(l.kp*(lms.donation/lm.amount)),2) donation,
  round(sum(if(lcd.balance_kp,lcd.balance_kp*(lms.donation/lm.amount),0)),2) balance,
  round(sum(if(lwo.principal,lwo.principal*(lms.donation/lm.amount),0)),2) write_off,
  z.zone, z.id
from
  (
    tblLoansMasterSponsors lms,
    tblLoansMaster lm,
    tblLoansMasterDetails lmd,
    tblLoans l,
    tblZones z
  )
left join
  tblLoansCurrentData lcd on lcd.loan_id = lmd.loan_id
left join
  tblLoanWriteOff lwo on lwo.loan_id = lmd.loan_id
where
  lms.sponsor_id = %s and
  lm.id = lms.master_id and
  lm.check_status = 'R' and
  lmd.master_id = lms.master_id and
  l.id = lmd.loan_id and
  z.id = lm.zone_id
group by
  lm.zone_id
) x1,
(
select
  sum(round((if(p.principal,p.principal,0))*(lms.donation/lm.amount),2)) principal,
  sum(round((if(p.interest,p.interest,0))*(lms.donation/lm.amount),2)) interest,
  sum(round((if(p.fees,p.fees,0))*(lms.donation/lm.amount),2)) fees,
  sum(round((if(p.insurances,p.insurances,0))*(lms.donation/lm.amount),2)) insurances,
  sum(round((if(p.penalties,p.penalties,0))*(lms.donation/lm.amount),2)) penalties,
  lm.zone_id
from
  (
    tblLoansMasterSponsors lms,
    tblLoansMaster lm,
    tblLoansMasterDetails lmd,
    tblLoans l
  )
left join
  tblPayments p on p.loan_id = lmd.loan_id
where
  lms.sponsor_id = %s and
  lm.id = lms.master_id and
  lm.check_status = 'R' and
  lmd.master_id = lms.master_id and
  l.id = lmd.loan_id
group by
  lm.zone_id
) x2
where
  x2.zone_id = x1.id",$_REQUEST['id'],$_REQUEST['id']));

$mrow['total']['zone'] = WEBPAGE::$gt['total'];

foreach($data as $key=>$val)
{
  $mrow['total']['borrowers'] += $val['borrowers'];
  $mrow['total']['loans']     += $val['loans'];
  $mrow['total']['donation']  += $val['donation'];
  $mrow['total']['principal'] += $val['principal'];
  $mrow['total']['write_off'] += $val['write_off'];
  $mrow['total']['balance']   += $val['balance'];
  $mrow['total']['profit']    += $val['interest'] + $val['fees'] + $val['penalties'];

  $mrow[$key]['zone']          = $val['zone'];
  $mrow[$key]['borrowers']     = $val['borrowers'];
  $mrow[$key]['loans']         = $val['loans'];
  $mrow[$key]['donation']      = $val['donation'];
  $mrow[$key]['principal']     = $val['principal'];
  $mrow[$key]['write_off']     = $val['write_off'];
  $mrow[$key]['balance']       = $val['balance'];
  $mrow[$key]['profit']        = $val['interest'] + $val['fees'] + $val['penalties'];
}

$head = array
(
  'zone'      => WEBPAGE::$gt['tblZones.zone'],
  'borrowers' => WEBPAGE::$gt['clients'],
  'loans'     => WEBPAGE::$gt['loans'],
  'donation'  => WEBPAGE::$gt['amount_disbursed'],
  'principal' => WEBPAGE::$gt['principalRepayment'],
  'write_off' => WEBPAGE::$gt['writeOff_amount'],
  'balance'   => WEBPAGE::$gt['portfolio_balance'],
  'profit'    => WEBPAGE::$gt['operationalIncome']
);

$mrow = array_reverse($mrow);

  $tpl->setVariable('sponsor_name_label',WEBPAGE::$gt['tblSponsors.sponsor']);
  $tpl->setVariable('sponsor_name_value',$sponsor[0]['sponsor']);
  $tpl->setVariable('chart', count($mrow)>1 ? WEBPAGE::printchart($mrow,$head) : WEBPAGE::$gt['noData']);

?>
