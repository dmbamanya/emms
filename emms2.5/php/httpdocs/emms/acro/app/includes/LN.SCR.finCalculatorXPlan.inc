<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'Date.php';

$loan = new LOAN(0,$_GET['id']);

$loan->data['kp']                = $_GET['kp'];
$loan->data['kat']               = $loan->data['kp'] * $_GET['fees_at'] / 100;
$loan->data['kaf']               = $loan->data['kp'] * $_GET['fees_af'] / 100;
$loan->data['P_KP']              = ($loan->data['kp']/($loan->data['kp']+$loan->data['kaf']+$loan->data['kat']));
$loan->data['P_KAF']             = ($loan->data['kaf']/($loan->data['kp']+$loan->data['kaf']+$loan->data['kat']));
$loan->data['P_KAT']             = ($loan->data['kat']/($loan->data['kp']+$loan->data['kaf']+$loan->data['kat']));
$loan->data['balance_kp']        = $loan->data['kp'];
$loan->data['balance_kat']       = $loan->data['kat'];
$loan->data['balance_kaf']       = $loan->data['kaf'];
$loan->data['rates_r']           = $_GET['rates_r'];
$loan->data['installment']       = $_GET['installment'];
$loan->data['calendar_type']     = $_GET['calendar_type'];
$loan->data['payment_frequency'] = $_GET['payment_frequency'];
$loan->data['rates_e']           = round(CalEffRate($loan->data['rates_r'],$loan->data['fees_at'],$loan->data['fees_af'],$loan->data['installment'],$loan->data['calendar_type'],$loan->data['payment_frequency']),2);
$loan->data['pmt']               = ceil(CalMinPMT($loan->data['kp'],$loan->data['kat'],$loan->data['kaf'],$loan->data['rates_r'],$loan->data['installment'],$loan->data['calendar_type'],$loan->data['payment_frequency']));
$loan->data['xp_pmt_date']       = $_GET['fpdate'] ? $_GET['fpdate'] : $loan->next_payment_date($_GET['rdate']);
$loan->data['xp_pmt']            = $loan->data['pmt'];
$loan->data['r_from_date']       = $_GET['rdate'];

$totals = array('inx'=>WEBPAGE::$gt['total'],'date'=>' ','fees'=>0,'insurances'=>0,'principal'=>0,'interest'=>0,'pmt'=>' ','balance_kp'=>' ');
$c = 0;

$balance_kp = $loan->data['balance_kp'];
while($balance_kp > 0)
{
  $data[$c] = $loan->getNextPaymentData();

  $loan->data['xp_pmt_date'] = $data[$c]['xp_pmt_date'];
  $loan->data['xp_pmt']      = $data[$c]['xp_pmt'];
  $loan->data['balance_kp']  = $data[$c]['balance_kp'];
  $loan->data['balance_kaf'] = $data[$c]['balance_kaf'];
  $loan->data['balance_kat'] = $data[$c]['balance_kat'];
  $loan->data['r_from_date'] = $data[$c]['r_from_date'];

  $pmt[$c]['inx']            = $c + 1;
  $pmt[$c]['date']           = $data[$c]['pmt_date'];
  $pmt[$c]['fees']           = $data[$c]['fees'];
  $pmt[$c]['insurances']     = $data[$c]['insurances'];
  $pmt[$c]['principal']      = $data[$c]['principal'];
  $pmt[$c]['interest']       = $data[$c]['interest'];

  $pmt[$c]['pmt']            = $data[$c]['pmt'];
  $pmt[$c]['balance_kp']     = $data[$c]['balance_kp'];

  $totals['fees']           += $data[$c]['fees'];
  $totals['insurances']     += $data[$c]['insurances'];
  $totals['principal']      += $data[$c]['principal'];
  $totals['interest']       += $data[$c]['interest'];

  $totals['pmt']            += $data[$c]['pmt'];

  $balance_kp                =  $data[$c]['balance_kp'];
  $c++;
}

$pmt[$c] = $totals;

$ltype = new LOAN_TYPE($loan->data['loan_type_id']);

$tpl->setVariable('pmtPlan_label',           WEBPAGE::$gt['LN.SCR.finCalculatorXPlan']);
$tpl->setVariable('kp',                      $loan->data['kp']);
$tpl->setVariable('kp_label',                WEBPAGE::$gt['tblLoans.kp']);
$tpl->setVariable('delivered_date',          $_GET['rdate']);
if ($_GET['fpdate'])
{
  $tpl->setVariable('first_pmt_date',        $_GET['fpdate']);
  $tpl->setVariable('first_pmt_date_label',  WEBPAGE::$gt['tblLoans.first_payment_date']);
}
$tpl->setVariable('delivered_date_label',    WEBPAGE::$gt['tblLoans.delivered_date']);

$tpl->setVariable('modality',                sprintf('%s : %s',WEBPAGE::$gt[sprintf('tblLoanTypes.borrower_type.%s',$ltype->data['borrower_type'])],$ltype->data['description']));
$tpl->setVariable('modality_label',          WEBPAGE::$gt['tblLoanTypes.description']);
$tpl->setVariable('installment',             $ltype->data['installment']);
$tpl->setVariable('installment_label',       WEBPAGE::$gt['tblLoanTypes.installment']);
$tpl->setVariable('payment_frequency',       WEBPAGE::$gt[sprintf('tblLoanTypes.payment_frequency.%s',$ltype->data['payment_frequency'])]);
$tpl->setVariable('payment_frequency_label', WEBPAGE::$gt['tblLoanTypes.payment_frequency']);
$tpl->setVariable('rates_r',                 $ltype->data['rates_r']);
$tpl->setVariable('rates_r_label',           WEBPAGE::$gt['tblLoanTypes.rates_r']);
$tpl->setVariable('fees_at',                 $ltype->data['fees_at']);
$tpl->setVariable('fees_at_label',           WEBPAGE::$gt['tblLoanTypes.fees_at']);
$tpl->setVariable('fees_af',                 $ltype->data['fees_af']);
$tpl->setVariable('fees_af_label',           WEBPAGE::$gt['tblLoanTypes.fees_af']);
$tpl->setVariable('rates_d',                 $ltype->data['rates_d']);
$tpl->setVariable('rates_d_label',           WEBPAGE::$gt['tblLoanTypes.rates_d']);
$tpl->setVariable('margin_d',                $ltype->data['margin_d']);
$tpl->setVariable('margin_d_label',          WEBPAGE::$gt['tblLoanTypes.margin_d']);
$tpl->setVariable('savings_p',               $ltype->data['savings_p']);
$tpl->setVariable('savings_p_label',         WEBPAGE::$gt['tblLoanTypes.savings_p']);

$tpl->setVariable('back',                    sprintf('<a href="index.popup.php?scr_name=LN.SCR.finCalculator&kp=%s&rdate=%s&id=%s">%s</a>',$_GET['kp'],$_GET['rdate'],$_GET['id'],WEBPAGE::$gt['back']));
$tpl->setVariable('print',                   sprintf('<a href="javascript:print()">%s</a>',WEBPAGE::$gt['printSheet']));

$head = array
        (
          'inx'        => WEBPAGE::$gt['tblPayments.id'],
          'date'       => WEBPAGE::$gt['tblPayments.date'],
          'fees'       => WEBPAGE::$gt['tblPayments.fees'],
          'insurances' => WEBPAGE::$gt['tblPayments.insurances'],
          'principal'  => WEBPAGE::$gt['tblPayments.principal'],
          'interest'   => WEBPAGE::$gt['tblPayments.interest'],
          'pmt'        => WEBPAGE::$gt['tblPayments.pmt'],
          'balance_kp' => WEBPAGE::$gt['tblPayments.balance_kp']
        );

$tpl->setVariable('chart', WEBPAGE::printchart($pmt,$head));

?>

