<?php
/*
 * being defensive against hacking... only loan master with all loans in status = 'O' or 'RO' can be revised
 */
if (current(current(WEBPAGE::$dbh->getAll(sprintf("select count(l.id) from tblLoans as l, tblLoansMasterDetails as lmd where l.status not in ('O','RO') and lmd.master_id = %s and lmd.loan_id = l.id",$_REQUEST['id'])))))
{
  WEBPAGE::redirect("index.popup.php?scr_name=exit");
  exit;
}

require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';

$btn  = new TTFButton(WEBPAGE::$theme);
$form = new HTML_QuickForm('revisionForm','POST');

$form->addElement('static',   'submit', '', $btn->imgsubmit('revisionForm',WEBPAGE::$gt['submit']));
$form->addElement('static',   'reset',  '', $btn->imgreset('revisionForm',WEBPAGE::$gt['reset']));
$form->addElement('static',   'cancel', '', $btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));
$form->addElement('hidden',   'scr_name',   $_REQUEST['scr_name']);
$form->addElement('hidden',   'id',         $_REQUEST['id']);
$form->addElement('textarea', 'memo',       WEBPAGE::$gt['memo'], 'rows=5');

$form->addRule('memo', sprintf(WEBPAGE::$gt['maxlength'],255),'maxlength', 255);

if ($form->validate())
{
  $form->freeze();
  $values = $form->_submitValues;
  $new_loan_status = 'S'; //Revised. Note that check status remains the same (pending)

  /*
   * Insert loan status log
   */
  WEBPAGE::$dbh->query(sprintf("insert into tblLoanStatusHistory (loan_id,p_status,status,date,user_id,memo) select l.id,l.status,'%s','%s','%s','%s' from tblLoans l, tblLoansMasterDetails lmd where lmd.master_id = %s and lmd.loan_id = l.id",$new_loan_status,date('Y-m-d'),WEBPAGE::$userID,$values['memo'],$values['id']));
  /*
   * update loans status
   */
  WEBPAGE::$dbh->query(sprintf("update tblLoans as l set l.status = '%s' where l.id in (select lmd.loan_id from tblLoansMasterDetails as lmd where lmd.master_id = %s)",$new_loan_status,$values['id']));
  /*
   * no need to update check status, it remains pending
   */

  /*
   * refresh parent window with a javascript call
   */
  $tpl->setVariable('refreshParent','refreshParent();');

} else {

  /*
   * print the form
   */
  $form->setDefaults();
  $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
  $renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
  $renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
  $form->accept($renderer);

  $tpl->setVariable('form_name', WEBPAGE::$gt['LN.SCR.doCheckRevision']);
  $tpl->setVariable('div_xls_download', 'remove');
  $tpl->setVariable('msg_label', WEBPAGE::$gt['important']);
  $tpl->setVariable('msg_text',  WEBPAGE::$gt['check_details_to_submit']);

  $tpl->setVariable('clickToRejectRequest', WEBPAGE::$gt['clickToRejectRequest']);
  $tpl->setVariable('clickToRetractRequest', WEBPAGE::$gt['clickToRetractRequest']);
  $tpl->setVariable('master_id', $_REQUEST['id']);
  $tpl->setVariable('more_options', WEBPAGE::$gt['more_options']);
  require_once './includes/LN.SCR.viewLoanMasterPrintable.inc';
}

?>

