<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/coolbutton.php';
require_once 'class/loan_master.php';
require_once 'class/client.php';
require_once 'class/society.php';
require_once 'class/sql.php';

$master = new LOAN_MASTER($_REQUEST['id']);
$master->loadBorrowerData();

$form = new HTML_QuickForm('editCheckNumberForm','POST');
$btn  = new TTFButton(WEBPAGE::$theme);

$form->addElement('static', 'submit', '',   $btn->imgsubmit('editCheckNumberForm',WEBPAGE::$gt['submit']));
$form->addElement('hidden', 'scr_name',     $_REQUEST['scr_name']);
$form->addElement('hidden', 'id',           $_REQUEST['id']);
$form->addElement('text',   'check_number', WEBPAGE::$gt['tblLoansMaster.check_number']);

$form->registerRule('checkcode', 'callback', 'checkcode');

$form->addRule('check_number',   sprintf(WEBPAGE::$gt['rangelength'],4,32),'required');
$form->addRule('check_number', 	 sprintf(WEBPAGE::$gt['rangelength'],4,32), 'checkcode', true);

if ($master->data['check_status'] != 'D')
{
    $form->removeElement('submit');
    $form->freeze();
}


if ($form->validate())
{
    $values = $form->_submitValues;
    $master->data['check_number'] = $values['check_number'];
    $master->updateCheckNumber();
    $tpl->setVariable('refreshParent','refreshParent();');
}

$tpl->setVariable('msg_label',           WEBPAGE::$gt['important']);
$tpl->setVariable('msg_text',            WEBPAGE::$gt['tips']['edit_check_number']);
$tpl->setVariable('borrower_label',      WEBPAGE::$gt['tblLoansMaster.borrower_id']);
$tpl->setVariable('borrower',            $master->data['borrower']->data['name']);
$tpl->setVariable('amount_label',        WEBPAGE::$gt['tblLoansMaster.amount']);
$tpl->setVariable('amount',              $master->data['amount']);

$tpl->setVariable('form_title',	WEBPAGE::$gt['LN.SCR.editCheckNumber']);

$form->setDefaults($master->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

function checkcode($code)
{
  $c = explode(' ', $code);
  if (count($c) > 1) return false;
  return true;
}
?>
