<?php
/*
 * being defensive against hacking... only loan master with all loans in status = 'S' can be approved
 */
if (current(current(WEBPAGE::$dbh->getAll(sprintf("select count(l.id) from tblLoans as l, tblLoansMasterDetails as lmd where l.status != 'S' and lmd.master_id = %s and lmd.loan_id = l.id",$_REQUEST['id'])))))
{
  WEBPAGE::redirect("index.popup.php?scr_name=exit");
  exit;
}
  
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';

$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('approvalForm','POST');

$form->addElement('static',   'submit', '', $btn->imgsubmit('approvalForm',WEBPAGE::$gt['submit']));
$form->addElement('static',   'reset',  '', $btn->imgreset('approvalForm',WEBPAGE::$gt['reset']));
$form->addElement('static',   'cancel', '', $btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));
$form->addElement('hidden',   'scr_name',   $_REQUEST['scr_name']);
$form->addElement('hidden',   'id',         $_REQUEST['id']);
$form->addElement('textarea', 'memo',       WEBPAGE::$gt['memo'], 'rows=5');

$form->addRule('memo', sprintf(WEBPAGE::$gt['maxlength'],255),'maxlength', 255);

if ($form->validate())
{   
  $form->freeze();  
  $values = $form->_submitValues;
  $new_loan_status        = 'A'; //Approved
  $new_loan_master_status = 'A'; //Approved
	
  /*
   * Insert loan status log
   */
  WEBPAGE::$dbh->query(sprintf("insert into tblLoanStatusHistory (loan_id,p_status,status,date,user_id,memo) select l.id,l.status,'%s','%s','%s','%s' from tblLoans as l, tblLoansMasterDetails as lmd where lmd.master_id = %s and lmd.loan_id = l.id",$new_loan_status,date('Y-m-d'),WEBPAGE::$userID,$values['memo'],$values['id']));
  /*
   * Update loans status
   */
  WEBPAGE::$dbh->query(sprintf("update tblLoans l set l.status = '%s' where l.id in (select lmd.loan_id from tblLoansMasterDetails lmd where lmd.master_id = %s)",$new_loan_status,$values['id']));
  /*
   * update check status
   */
  WEBPAGE::$dbh->query(sprintf("update tblLoansMaster set check_status = '%s' where id = %s",$new_loan_master_status,$values['id']));  

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

  $tpl->setVariable('form_name', WEBPAGE::$gt['LN.SCR.doCheckApproval']);
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
