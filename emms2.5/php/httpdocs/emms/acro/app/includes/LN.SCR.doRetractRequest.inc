<?php
/*
 * being defensive against hacking... only loan master with all loans in status 'O', 'RO', 'S', 'D', or 'R' can be retracted
 */
$status = WEBPAGE::$dbh->getAll(sprintf("
            select
              l.status
            from
              tblLoans l,
              tblLoansMasterDetails lmd
            where
              lmd.master_id = %s and
              lmd.loan_id   = l.id", $_REQUEST['id']));

foreach($status as $key => $val)
{
  $st[$val['status']] = $val['status'];
}

$sts = current($st);
if ((count($st) > 1)||(($sts != 'O')&&($sts != 'RO')&&($sts != 'S')&&($sts != 'D')&&($sts != 'R')))
{
  WEBPAGE::redirect("index.popup.php?scr_name=exit");
  exit;
}

require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';

$btn  = new TTFButton(WEBPAGE::$theme);
$form = new HTML_QuickForm('revisionForm','POST');

$form->addElement('static',   'submit', '', $btn->imgsubmit('revisionForm',WEBPAGE::$gt['fn']['LN.SCR.doRetractRequest']));
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

  /*
   * Insert loan status log
   */
  WEBPAGE::$dbh->query(sprintf("
    insert into tblLoanStatusHistory (loan_id,p_status,status,date,user_id,memo) select l.id,l.status,'%s','%s','%s','%s' from tblLoans l, tblLoansMasterDetails lmd where lmd.master_id = %s and lmd.loan_id = l.id",'RT',date('Y-m-d'),WEBPAGE::$userID,$values['memo'],$values['id']));
  /*
   * update loans status
   */
  WEBPAGE::$dbh->query(sprintf("update tblLoans as l set l.status = '%s' where l.id in (select lmd.loan_id from tblLoansMasterDetails as lmd where lmd.master_id = %s)",'RT',$values['id']));
  /*
   * update loan master status
   */
  WEBPAGE::$dbh->query(sprintf("update tblLoansMaster lm set lm.check_status = '%s' where lm.id = '%s'",'RT',$values['id']));
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

  $tpl->setVariable('form_name',        WEBPAGE::$gt['LN.SCR.doRetractRequest']);
  $tpl->setVariable('div_xls_download', 'remove');
  $tpl->setVariable('msg_label',        WEBPAGE::$gt['important']);
  $tpl->setVariable('msg_text',         WEBPAGE::$gt['check_details_to_submit']);
  $tpl->setVariable('retract_request',  WEBPAGE::$gt['tips']['retract_request']);
  
  require_once './includes/LN.SCR.viewLoanMasterPrintable.inc';
}

?>