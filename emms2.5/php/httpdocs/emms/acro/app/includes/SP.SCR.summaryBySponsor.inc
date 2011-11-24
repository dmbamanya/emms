<?php
require_once "Spreadsheet/Excel/Writer.php";

$sponsors = WEBPAGE::$dbh->getAll(sprintf("
  select
    s.id,
    s.sponsor,
    concat(s.first, ' ',s.last) contact,
    s.email,
    sum(sd.conv_amount) donations,
    sum(sd.conv_tip) tips
  from
    tblSponsors s,
    tblSponsorsDonations sd
  where
    sd.sponsor_id = s.id
  group by
    s.id"));

$pmt = WEBPAGE::$dbh->getAssoc("
  select
    lms.sponsor_id,
    sum(round((if(p.principal,p.principal,0))*(lms.donation/lm.amount),2)) principal,
    sum(round((if(p.interest,p.interest,0))*(lms.donation/lm.amount),2)) interest,
    sum(round((if(p.fees,p.fees,0))*(lms.donation/lm.amount),2)) fees,
    sum(round((if(p.insurances,p.insurances,0))*(lms.donation/lm.amount),2)) insurance,
    sum(round((if(p.penalties,p.penalties,0))*(lms.donation/lm.amount),2)) penalties
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
    lm.id = lms.master_id and
    lm.check_status = 'R' and
    lmd.master_id = lms.master_id and
    l.id = lmd.loan_id
  group by
    lms.sponsor_id");

$portfolio = WEBPAGE::$dbh->getAssoc("
  select
    lms.sponsor_id,
    count(distinct lmd.loan_id) borrowers,
    count(distinct lms.master_id) loans,
    round(sum(l.kp*(lms.donation/lm.amount)),2) disbursement,
    round(sum(if(lcd.balance_kp,lcd.balance_kp*(lms.donation/lm.amount),0)),2) balance,
    round(sum(if(lwo.principal,lwo.principal*(lms.donation/lm.amount),0)),2) write_off
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
    lms.sponsor_id");

$c = 0;
foreach($sponsors as $key=>$val)
{
    $c++;

    //$mrow[$key]['sponsor_id']   = $c;
    $mrow[$key]['sponsor']      = $val['sponsor'];
    $mrow[$key]['contact']      = $val['contact'];
    $mrow[$key]['email']        = $val['email'];
    $mrow[$key]['tips']         = $val['tips'] + 0;
    $mrow[$key]['donations']    = $val['donations'];
    $mrow[$key]['loans']        = $portfolio[$val['id']]['loans'];
    $mrow[$key]['clients']      = $portfolio[$val['id']]['borrowers'];
    $mrow[$key]['disbursement'] = $portfolio[$val['id']]['disbursement'];
    $mrow[$key]['repayment']    = $pmt[$val['id']]['principal'];
    $mrow[$key]['write_off']    = $portfolio[$val['id']]['write_off'];
    $mrow[$key]['balance']      = $portfolio[$val['id']]['balance'];
    $mrow[$key]['insurance']    = $pmt[$val['id']]['insurance'];
    $mrow[$key]['profit']       = $pmt[$val['id']]['interest'] + $pmt[$val['id']]['fees'] + $pmt[$val['id']]['penalties'];

    //$total['sponsor_id']   = WEBPAGE::$gt['total'];
    $total['sponsor']      = '';
    $total['contact']      = '';
    $total['email']        = '';
    $total['tips']         += $mrow[$key]['tips'];
    $total['donations']    += $mrow[$key]['donations'];
    $total['loans']        += $mrow[$key]['loans'];
    $total['clients']      += $mrow[$key]['clients'];
    $total['disbursement'] += $mrow[$key]['disbursement'];
    $total['repayment']    += $mrow[$key]['repayment'];
    $total['write_off']    += $mrow[$key]['write_off'];
    $total['balance']      += $mrow[$key]['balance'];
    $total['insurance']    += $mrow[$key]['insurance'];
    $total['profit']       += $mrow[$key]['profit'];
}

//$mrow[$c++] = $total;

$head = array
(
  //'sponsor_id'   => '#',
  'sponsor'      => WEBPAGE::$gt['tblSponsors.sponsor'],
  'contact'      => WEBPAGE::$gt['tblSponsors.contact'],
  'email'        => WEBPAGE::$gt['tblClients.email'],
  'tips'         => WEBPAGE::$gt['donations_paypal_tipname'],
  'donations'    => WEBPAGE::$gt['donations'],
  'loans'        => WEBPAGE::$gt['loans'],
  'clients'      => WEBPAGE::$gt['clients'],
  'disbursement' => WEBPAGE::$gt['amount_disbursed'],
  'repayment'    => WEBPAGE::$gt['principalRepayment'],
  'write_off'    => WEBPAGE::$gt['writeOff_amount'],
  'balance'      =>  WEBPAGE::$gt['portfolio_balance'],
  'insurance'    => WEBPAGE::$gt['insurance'],
  'profit'       => WEBPAGE::$gt['operationalIncome']
);

array_unshift($mrow,$head);

$xls =& new Spreadsheet_Excel_Writer();
$xls->send("summary.xls");
$sheet =& $xls->addWorksheet('Sheet 1');

$row = 0;
foreach ( $mrow as $key => $val) {
  $col = 0;
   foreach ($val as $i => $j) {
   	  $sheet->write($row,$col,$j);
   	  $col++;
   	  }
 $row++;
}

$xls->close();

//$tpl->setVariable('script_name_label',WEBPAGE::$gt['RP.SCR.performance']);
//$tpl->setVariable('chart', count($mrow) > 1 ? WEBPAGE::printchart($mrow,$head) : WEBPAGE::$gt['noData']);
?>