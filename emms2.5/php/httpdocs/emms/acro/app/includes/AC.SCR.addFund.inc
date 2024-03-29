<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/fund.php';

$fund = new FUND($_REQUEST['id']);
$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('fundForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('fundForm',WEBPAGE::$gt['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('fundForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);

$form->addElement('text',		'code',			WEBPAGE::$gt['tblFunds.code'], 'class=large');
$form->addElement('text',		'fund',			WEBPAGE::$gt['tblFunds.fund'], 'class=large');

$form->addElement('select',		'currency_id',	WEBPAGE::$gt['tblFunds.currency_id'],	$fund->currencys(), 'class=large');

$form->addElement('textarea',	'description',	WEBPAGE::$gt['tblFunds.description']);

$status_radio[] = $form->createElement('radio',null,null,WEBPAGE::$gt['tblFunds.status.A'],'A', 'class=radio');
$status_radio[] = $form->createElement('radio',null,null,WEBPAGE::$gt['tblFunds.status.I'],'I', 'class=radio');
$form->addGroup($status_radio, 'status', WEBPAGE::$gt['tblFunds.status'],' ');

//add rules
$form->addRule('fund',			sprintf(WEBPAGE::$gt['rangelength'],1,32),'required');
$form->addRule('fund',			sprintf(WEBPAGE::$gt['rangelength'],1,32),'rangelength', array(1,32));
$form->addRule('fund',			WEBPAGE::$gt['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('code',			sprintf(WEBPAGE::$gt['rangelength'],1,32),'required');
$form->addRule('code',			sprintf(WEBPAGE::$gt['rangelength'],1,32),'rangelength', array(1,32));
$form->addRule('code',			WEBPAGE::$gt['alphanumeric'],'alphanumeric');
$form->addRule('description',	sprintf(WEBPAGE::$gt['maxlength'],255),'maxlength', 255);

$form->freeze($fund->frostdata);

if ($form->validate()) { 
  
  $form->freeze();
  
  $values = $form->_submitValues;
  
  $values['editor_date']  = date('Y-m-d');
  $values['creator_date'] = $fund->funddata['creator_date'] ? $fund->funddata['creator_date'] : $values['editor_date'];
  $values['editor_id']    = WEBPAGE::$userID;
  $values['creator_id']   = $fund->funddata['creator_id']   ? $fund->funddata['creator_id']   : $values['editor_id'];
  
  $lastID = SQL::insert('tblFunds',$values,'id');
  
  WEBPAGE::redirect(sprintf('index.php?scr_name=AC.SCR.viewFund&id=%s&ico=scs&msg=scs',$lastID));

  } 

$form->setDefaults($fund->funddata);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>