<?php
/*
 * being defensive against hacking... only loan master with all loans in status = 'D' can be released
 */
if (current(current(WEBPAGE::$dbh->getAll(sprintf("select count(l.id) from tblLoans l, tblLoansMasterDetails lmd where l.status != 'D' and lmd.master_id = %s and lmd.loan_id = l.id",$_REQUEST['id'])))))
{
  WEBPAGE::redirect("index.popup.php?scr_name=exit");
  exit;
}

require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';

require_once 'Date.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/client.php';
require_once 'class/society.php';

$master = new LOAN_MASTER($_REQUEST['id']);
$btn    = new TTFButton(WEBPAGE::$theme);
$form   = new HTML_QuickForm('releaseForm','POST');

$master->loadloans();

$form->addElement('static',   'submit',             '', $btn->imgsubmit('releaseForm',WEBPAGE::$gt['submit']));
$form->addElement('static',   'reset',              '', $btn->imgreset('releaseForm',WEBPAGE::$gt['reset']));
$form->addElement('static',   'cancel',             '', $btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));
$form->addElement('hidden',   'scr_name',               $_REQUEST['scr_name']);
$form->addElement('hidden',   'id',                     $_REQUEST['id']);
$form->addElement('hidden',   'min_delivered_date',     $_REQUEST['min_delivered_date']);
$form->addElement('hidden',   'max_first_payment_date', $_REQUEST['max_first_payment_date']);
$form->addElement('text',     'delivered_date',         WEBPAGE::$gt['tblLoans.delivered_date'], 'class=large');
$form->addElement('text',     'first_payment',          WEBPAGE::$gt['tblLoans.first_payment_date'], 'class=large');
$form->addElement('textarea', 'memo',                   WEBPAGE::$gt['memo'], 'rows=3');

$form->addRule('delivered_date', 'yyyy-mm-dd','required');
$form->addRule('delivered_date', 'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('delivered_date', WEBPAGE::$gt['nonValidDate'],'callback','checkdates_delivered');
$form->addRule('first_payment',  'yyyy-mm-dd','required');
$form->addRule('first_payment',  'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('first_payment',  WEBPAGE::$gt['nonValidDate'],'callback','checkdates_1stpmt');
$form->addRule('memo',           sprintf(WEBPAGE::$gt['maxlength'],255),'maxlength', 255);

if ($form->validate())
{
  $form->freeze();
  $values = $form->_submitValues;
  $new_loan_status        = 'G'; //Released
  $new_loan_master_status = 'R'; //Released

  /*
   * Insert loan status log
   */
  WEBPAGE::$dbh->query(sprintf("insert into tblLoanStatusHistory (loan_id,p_status,status,date,user_id,memo) select l.id,l.status,'%s','%s','%s','%s' from tblLoans as l, tblLoansMasterDetails as lmd where lmd.master_id = %s AND lmd.loan_id = l.id",$new_loan_status,date('Y-m-d'),WEBPAGE::$userID,$values['memo'],$values['id']));
  /*
   * update loans status
   */
  WEBPAGE::$dbh->query(sprintf("update tblLoans as l set l.status = '%s',l.delivered_date = '%s',l.first_payment_date = '%s' where l.id in (select lmd.loan_id from tblLoansMasterDetails as lmd where lmd.master_id = %s)",$new_loan_status,$values['delivered_date'],$values['first_payment'],$values['id']));
  /*
   * update check status
   */
  WEBPAGE::$dbh->query(sprintf("update tblLoansMaster set check_status = '%s' where id = %s",$new_loan_master_status,$values['id']));
  /*
   * insert TDebits data para fines de conciliaciones
   */
  WEBPAGE::$dbh->query(sprintf("insert into tblTDebits (id,code,date,branch_id,program_id,amount) select lm.id,lm.check_number,CURDATE(),lm.zone_id,lm.program_id,lm.amount from tblLoansMaster as lm where lm.id = '%s'",$values['id']));
  /*
   * Insert loan current data
   */
  WEBPAGE::$dbh->query(sprintf("insert into tblLoansCurrentData (loan_id,balance_kp,balance_kaf,balance_kat,r_from_date,xp_pmt_date,xp_pmt,cn_date,cn_delay,cn_penalties) select l.id,l.kp,l.kaf,l.kat,l.delivered_date,l.first_payment_date,l.pmt,l.delivered_date,0,0 from tblLoans as l, tblLoanTypes as lt, tblLoansMasterDetails as lmd where lmd.master_id = %s AND lmd.loan_id = l.id AND lt.id = l.loan_type_id",$values['id']));
  /*
   * update the loan expected number of payments and cancellation date
   */
  foreach ($master->data['loans'] as $loan_id => $status)
  {
    $loan = new LOAN($loan_id);
    $loan->load_xp_pmt_plan();

    $xp_num_pmt = count($loan->data['xp_pmt_plan']);
    $xp_cancel_date = $loan->data['xp_pmt_plan'][$xp_num_pmt-1]['pmt_date'];
    WEBPAGE::$dbh->query(sprintf("update tblLoans as l set l.xp_num_pmt = '%s',l.xp_cancel_date = '%s' where l.id = %s",$xp_num_pmt,$xp_cancel_date,$loan_id));
  }

  /*
   * refresh parent window with a javascript call
   */
  $tpl->setVariable('refreshParent','refreshParent();');
  
} else {

  /*
   * minimum delivered date: $mdate
   */
  $mdate = new Date();
  $mdate->subtractSeconds(24*60*60);
  /*
   * definitive delivered date: $def_delivered_date
   */
  $ddate = new Date();
  if (($master->data['xp_delivered_date'] <= $ddate->format('%Y-%m-%d')) && ($master->data['xp_delivered_date'] >= $mdate->format('%Y-%m-%d')))
  {
    $def_delivered_date = $master->data['xp_delivered_date'];
  } else {
    $def_delivered_date = $ddate->format('%Y-%m-%d');
  }
  /*
   * max. 1st paymente date: $max_1st_pmt_date
   */
  $master->loadBorrowerData();
  $loan = new LOAN(0,$master->data['loan_type_id']);
  $max_1st_pmt_date = new Date($loan->next_payment_date($def_delivered_date));
  /*
   * definitive 1st payment date: $fdate
   */
  if (WEBPAGE::$conf['app']['flex_1st_pmt_date'] == '1')
  {
    $fdate = ($master->data['xp_first_payment_date'] > $def_delivered_date ) ? new Date($master->data['xp_first_payment_date']) : $max_1st_pmt_date;
  } else {
    $fdate = (($master->data['xp_first_payment_date'] <= $loan->next_payment_date($def_delivered_date))&&($master->data['xp_first_payment_date'] > $def_delivered_date)) ? new Date($master->data['xp_first_payment_date']) : $max_1st_pmt_date;
  }
  /*
   * print the form
   */
  $form->setDefaults(array('max_first_payment_date'=>$max_1st_pmt_date->format('%Y-%m-%d'),'min_delivered_date'=>$mdate->format('%Y-%m-%d'),'delivered_date'=>$def_delivered_date,'first_payment'=>$fdate->format('%Y-%m-%d')));
  $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
  $renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
  $renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
  $form->accept($renderer);

  $tpl->setVariable('form_name', WEBPAGE::$gt['LN.SCR.doCheckDisbursement']);
  $tpl->setVariable('div_xls_download', 'remove');
  $tpl->setVariable('msg_label', WEBPAGE::$gt['important']);
  $tpl->setVariable('msg_text',  WEBPAGE::$gt['check_details_to_submit']);

  $tpl->setVariable('clickToRejectRequest', WEBPAGE::$gt['clickToRejectRequest']);
  $tpl->setVariable('clickToRetractRequest', WEBPAGE::$gt['clickToRetractRequest']);
  $tpl->setVariable('master_id', $_REQUEST['id']);
  $tpl->setVariable('more_options', WEBPAGE::$gt['more_options']);
  require_once './includes/LN.SCR.viewLoanMasterPrintable.inc';
}

function checkdates_1stpmt()
{
  $date_1stpmt = explode('-',$_REQUEST['first_payment']);
  if (!checkdate($date_1stpmt[1],$date_1stpmt[2],$date_1stpmt[0])) { return false; } //verifica que es una fecha real
  if ($_REQUEST['first_payment'] <= $_REQUEST['delivered_date']) { return false; }
  if (WEBPAGE::$conf['app']['flex_1st_pmt_date'] == '1') return true;
  if (date('w',date(mktime(0,0,0,$date_1stpmt[1],$date_1stpmt[2],$date_1stpmt[0]))) == 0) { return false; } // verifica que no sea domingo
  if (date('w',date(mktime(0,0,0,$date_1stpmt[1],$date_1stpmt[2],$date_1stpmt[0]))) == 6) { return false; } // verifica que no sea sabado
  if ($_REQUEST['first_payment'] > $_REQUEST['max_first_payment_date']) { return false; }
  return true;
}

function checkdates_delivered()
{
  $date_delivered = explode('-',$_REQUEST['delivered_date']);
  if (!checkdate($date_delivered[1],$date_delivered[2],$date_delivered[0])) { return false; } //verifica que es una fecha real
  if (date('w',date(mktime(0,0,0,$date_delivered[1],$date_delivered[2],$date_delivered[0]))) == 0) { return false; } // verifica que no sea domingo
  if (date('w',date(mktime(0,0,0,$date_delivered[1],$date_delivered[2],$date_delivered[0]))) == 6) { return false; } // verifica que no sea sabado
  if ($_REQUEST['delivered_date'] > date('Y-m-d')) { return false; }
  if ($_REQUEST['delivered_date'] < $_REQUEST['min_delivered_date']) { return false; }
  return true;
}

?>
