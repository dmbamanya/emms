<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/loan_master.php';

$btn   = new TTFButton(WEBPAGE::$theme);
$loan  = new LOAN(0,$_REQUEST['id']);

$ltype = new LOAN_TYPE($_REQUEST['id']);

$tpl->setVariable('title',          WEBPAGE::$gt['LN.SCR.finCalculatorX']);
$tpl->setVariable('loanType',       $ltype->data['description']);
$tpl->setVariable('loanType_label', WEBPAGE::$gt['tblLoanTypes.description']);
$tpl->setVariable('rates_label',    WEBPAGE::$gt['rate']);
$tpl->setVariable('margin_label',   WEBPAGE::$gt['margin']);
$tpl->setVariable('days',           WEBPAGE::$gt['days']);

$form = new HTML_QuickForm('iLoanForm','POST');

$form->addElement('static',   'submit',            '', $btn->imgsubmit('iLoanForm',WEBPAGE::$gt['submit']));

$form->addElement('hidden',   'scr_name',              $_REQUEST['scr_name']);
$form->addElement('hidden',   'id',                    $_REQUEST['id']);

$form->addElement('static',   'rate',              '', WEBPAGE::$gt['rate']);
$form->addElement('static',   'insurance',         '', WEBPAGE::$gt['insurance']);

$form->addElement('select',   'payment_frequency',     WEBPAGE::$gt['tblLoanTypes.payment_frequency'], $ltype->payment_frequencys(), "onChange='calEffRate()'");
$form->addElement('select',   'calendar_type',         WEBPAGE::$gt['tblLoanTypes.calendar_type'],     $ltype->calendar_types(),     "onChange='calEffRate()'");

$form->addElement('text',     'rdate',                 WEBPAGE::$gt['tblLoans.delivered_date']);
$form->addElement('text',     'fpdate',                WEBPAGE::$gt['tblLoans.first_payment_date']);
$form->addElement('text',     'kp',                    WEBPAGE::$gt['tblLoans.kp']);
$form->addElement('text',     'rates_r',               WEBPAGE::$gt['tblLoanTypes.rates_r'],                                         "onKeyUp='calEffRate()'");
$form->addElement('text',     'rates_e',               WEBPAGE::$gt['rate_e'],                                                       "onFocus='document.iLoanForm.rates_e.blur()' style='color:gray'");
$form->addElement('text',     'fees_at',               WEBPAGE::$gt['tblLoanTypes.fees_at'],                                         "onKeyUp='calEffRate()'");
$form->addElement('text',     'fees_af',               WEBPAGE::$gt['tblLoanTypes.fees_af'],                                         "onKeyUp='calEffRate()'");
$form->addElement('text',     'installment',           WEBPAGE::$gt['tblLoanTypes.installment'],                                     "onKeyUp='calEffRate()'");

//add rules
$form->addRule('rdate',       'yyyy-mm-dd','required');
$form->addRule('rdate',       'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('fpdate',      'yyyy-mm-dd','required');
$form->addRule('fpdate',      'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('fpdate',      WEBPAGE::$gt['nonValidDate'],'callback','checkdates_1stpmt');

$form->addRule('kp',          WEBPAGE::$gt['kp_err'],  'required');
$form->addRule('kp',          WEBPAGE::$gt['kp_err'],  'numeric');
$form->addRule('installment', WEBPAGE::$gt['numeric'], 'required');
$form->addRule('installment', WEBPAGE::$gt['numeric'], 'regex', '(^[0-9 ]*$)');
$form->addRule('rates_r',     WEBPAGE::$gt['numeric'], 'required');
$form->addRule('rates_r',     WEBPAGE::$gt['numeric'], 'numeric');
$form->addRule('fees_at',     WEBPAGE::$gt['numeric'], 'required');
$form->addRule('fees_at',     WEBPAGE::$gt['numeric'], 'numeric');
$form->addRule('fees_af',     WEBPAGE::$gt['numeric'], 'required');
$form->addRule('fees_af',     WEBPAGE::$gt['numeric'], 'numeric');

$form->freeze($loan->frostdata);

if ($form->validate())
{
  $values = $form->_submitValues;
  $file = sprintf('index.popup.php?scr_name=LN.SCR.finCalculatorXPlan&id=%s&rdate=%s&fpdate=%s&kp=%s&rates_r=%s&fees_at=%s&fees_af=%s&installment=%s&calendar_type=%s&payment_frequency=%s',$values['id'],$values['rdate'],$values['fpdate'],$values['kp'],$values['rates_r'],$values['fees_at'],$values['fees_af'],$values['installment'],$values['calendar_type'],$values['payment_frequency']);
  $tpl->setVariable('showPlan', sprintf('showPlan("%s")',$file));
}

$form->setDefaults($loan->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

function checkdates_1stpmt()
{
  $date_1stpmt = explode('-',$_REQUEST['fpdate']);
  if (!checkdate($date_1stpmt[1],$date_1stpmt[2],$date_1stpmt[0])) { return false; } //not a fake date
  if ($_REQUEST['fpdate'] <= $_REQUEST['rdate']) { return false; }
  if (WEBPAGE::$conf['app']['flex_1st_pmt_date'] == '1') return true;
  if (date('w',date(mktime(0,0,0,$date_1stpmt[1],$date_1stpmt[2],$date_1stpmt[0]))) == 0) { return false; } // not a Sunday
  if (date('w',date(mktime(0,0,0,$date_1stpmt[1],$date_1stpmt[2],$date_1stpmt[0]))) == 6) { return false; } // not a Saturday
  return true;
}
?>

