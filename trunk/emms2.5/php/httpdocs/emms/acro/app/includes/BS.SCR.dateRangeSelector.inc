<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';

$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('dateForm','POST');

$form->addElement('static', 	'submit', 	'', 					$btn->imgsubmit('dateForm',WEBPAGE::$gt['submit']));
$form->addElement('static', 	'oldrange_l', 						WEBPAGE::$gt['oldrange'], 	$_REQUEST['oldrange']);

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'oldrange',		$_REQUEST['oldrange']);
$form->addElement('hidden',		'href',			$_REQUEST['href']);

$form->addElement('text',		'date_from',	WEBPAGE::$gt['dateFrom'], 		$_REQUEST['date_from']);
$form->addElement('text',		'date_to',		WEBPAGE::$gt['dateTo'], 		$_REQUEST['date_to']);

//add rules
$form->addRule('date_from',	'yyyy-mm-dd','required');
$form->addRule('date_from',	'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('date_to',	'yyyy-mm-dd','required');
$form->addRule('date_to',	'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');

if ($form->validate()) { 
  
  $form->freeze();
  $values = $form->_submitValues; 
  WEBPAGE::redirect(sprintf('%s&date_from=%s&date_to=%s',$values['href'],$values['date_from'],$values['date_to']));
  
  } 

$tpl->setVariable('form_title',	WEBPAGE::$gt['BS.SCR.dateRangeSelector']);

$form->setDefaults();
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>
