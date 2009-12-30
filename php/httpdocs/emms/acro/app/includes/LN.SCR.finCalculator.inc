<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';

$data = WEBPAGE::$dbh->getAll('select id,borrower_type,description from tblLoanTypes order by borrower_type');
$loantypes = array();
foreach($data as $key=>$val) {
  $loantypes[$val['id']] = sprintf('%s : %s',WEBPAGE::$gt[sprintf('tblLoanTypes.borrower_type.%s',$val['borrower_type'])],$val['description']);
  }

$btn  = new TTFButton(WEBPAGE::$theme);
$form = new HTML_QuickForm('calForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('calForm',WEBPAGE::$gt['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('calForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('text',		'rdate',		WEBPAGE::$gt['tblLoans.delivered_date'], 'class=large');
$form->addElement('text',		'kp',			WEBPAGE::$gt['tblLoans.kp'], 'class=large');
$form->addElement('select',		'id',			WEBPAGE::$gt['tblLoanTypes.description'],	array(''=>'')+$loantypes, 'class=large');

//add rules
$form->addRule('id',			WEBPAGE::$gt['required'],'required');
$form->addRule('kp',			WEBPAGE::$gt['kp_err'],		'required');
$form->addRule('kp',			WEBPAGE::$gt['kp_err'],		'numeric');
$form->addRule('rdate',			'yyyy-mm-dd','required');
$form->addRule('rdate',			'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');

if ($form->validate()) { 
  
//  $form->freeze();
  
  $values = $form->_submitValues;
  
  WEBPAGE::redirect(sprintf('index.popup.php?scr_name=LN.SCR.finCalculatorPlan&id=%s&rdate=%s&kp=%s',$values['id'],$values['rdate'],$values['kp']));

  } 

$tpl->setVariable('page_title', WEBPAGE::$gt['LN.SCR.finCalculator']);
$form->setDefaults($_REQUEST);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>