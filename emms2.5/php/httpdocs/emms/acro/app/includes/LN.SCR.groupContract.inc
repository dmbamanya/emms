<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'class/receipt.php';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/client.php';
require_once 'class/society.php';
require_once 'Date.php';

$master 	= new LOAN_MASTER($_REQUEST['id']);

$master->loadloans();
$master->loadextrainfo();

/*
 * load contract template if exists in app_custom_tpl folder
 */
if ( file_exists(sprintf(WEBPAGE::_APP_CUSTOM_TPL,WEBPAGE::$paths['3'],WEBPAGE::$paths['2']).'/'.sprintf("%s.%s",$_REQUEST['tpl'],'tpl')))
{
  $tpl->setRoot(sprintf(WEBPAGE::_APP_CUSTOM_TPL,WEBPAGE::$paths['3'],WEBPAGE::$paths['2']));
  $tpl->loadTemplateFile(sprintf("%s.tpl",$_REQUEST['tpl']));
  $tpl->setRoot('./templates');
} else {
  $tpl->loadTemplateFile('contract.default.tpl');
  $tpl->setVariable('message',WEBPAGE::$gt['contract_error']);
}

$c = 1;
foreach($master->data['extrainfo'] as $loan_id => $data)
{
  $loan = new LOAN($loan_id);
  $loan->load_xp_pmt_plan(); 

  $number_of_pmt = count($loan->data['xp_pmt_plan']);
  $total_amount += $loan->data['xp_pmt_plan_totals']['pmt'];
  $pmt_last		+= $loan->data['xp_pmt_plan'][$number_of_pmt - 1]['pmt'];

  $tpl->setVariable(sprintf('client_%s',$c),   $data['client_name']);
  $tpl->setVariable(sprintf('code_%s',$c),     $data['client_code']);
  $tpl->setVariable(sprintf('address_%s',$c),  $data['client_address']);

  $pmt_amount += $data['pmt'];
  $payment_frequency = $data['payment_frequency'];

  $c++;
}

$master->loadBorrowerData();

$tpl->setVariable('master_id',          $master->data['id']);
$tpl->setVariable('borrower',           $master->data['borrower']->data['name']);
$tpl->setVariable('kp',                 number_format($master->data['amount'], 2, '.', ','));
$tpl->setVariable('pmt_amount',         number_format($pmt_amount, 2, '.', ','));
$tpl->setVariable('payment_frequency',  WEBPAGE::$gt[sprintf('tblLoanTypes.payment_frequency.%s',$payment_frequency)]);

$tpl->setVariable('num_payments',       $number_of_pmt);
$tpl->setVariable('pmt_last',           number_format($pmt_last, 2, '.', ','));
$tpl->setVariable('total_amount',       number_format($total_amount, 2, '.', ','));

$date_to_print = $loan->check_xp_delivered_date() ? $master->data['xp_delivered_date'] : date('Y-m-d');

$tpl->setVariable('date',               WEBPAGE::verbose_date_format($date_to_print));
$tpl->setVariable('fdate',              WEBPAGE::verbose_date_format($master->data['xp_first_payment_date']));


?>
