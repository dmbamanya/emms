<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/program.php';

$program = new PROGRAM($_REQUEST['id']);

$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('programForm','POST');

$form->addElement('static', 	'submit', 			'', $btn->imgsubmit('programForm',WEBPAGE::$gt['submit']));
$form->addElement('static', 	'reset', 			'', $btn->imgreset('programForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 	'cancel', 			'', $btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));

$form->addElement('hidden',		'scr_name', 		$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',       		$_REQUEST['id']);

$form->addElement('text',		'program',  		WEBPAGE::$gt['tblPrograms.program'],'class=large');
$form->addElement('advcheckbox','confidential',		'',	sprintf(" %s",WEBPAGE::$gt['tblPrograms.confidential']),'class=checkbox');

$form->addElement('select',		'fund_id',  		WEBPAGE::$gt['tblPrograms.fund_id'],	$program->funds(),			 	'multiple size="5" class=large');
$form->addElement('static',		'current_funds',	'', implode('<br>',$program->getFunds()));

$form->addElement('textarea',	'description',		WEBPAGE::$gt['tblPrograms.description']);

$status_radio[] = $form->createElement('radio',null,null,WEBPAGE::$gt['tblPrograms.status.A'],'A', 'class=radio');
$status_radio[] = $form->createElement('radio',null,null,WEBPAGE::$gt['tblPrograms.status.I'],'I', 'class=radio');

$form->addGroup($status_radio, 'status', 			WEBPAGE::$gt['tblPrograms.status'],' ');


//add rules
$form->addRule('program',		sprintf(WEBPAGE::$gt['rangelength'],1,32),'required');
$form->addRule('program',		sprintf(WEBPAGE::$gt['rangelength'],1,32),'rangelength', array(1,32));
$form->addRule('program',		WEBPAGE::$gt['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('description',	sprintf(WEBPAGE::$gt['maxlength'],255),'maxlength', 255);

$form->freeze($program->frostdata);

if ($form->validate()) { 
  
  $form->freeze();
  
  $values = $form->_submitValues;
  
  $values['editor_date']  = date('Y-m-d');
  $values['creator_date'] = $program->programdata['creator_date'] ? $program->programdata['creator_date'] : $values['editor_date'];
  $values['editor_id']    = WEBPAGE::$userID;
  $values['creator_id']   = $program->programdata['creator_id']   ? $program->programdata['creator_id']   : $values['editor_id'];
  
  $fund_id = $values['fund_id'];
  unset($values['fund_id']);
  
  $lastID = SQL::insert('tblPrograms',$values,'id');
  
  if (is_array($fund_id)) {
    $comma = ' ';
    $data = '';
	for ($i=0;$i<count($fund_id);$i++) {
	  $data .= sprintf("%s(%s,%s)", $comma, $lastID, $fund_id[$i]);
	  $comma = ',';
	  }
    SQL::insert_mult('tblLinkProgramsFunds','program_id,fund_id', $data);
    }

  WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewProgram&id=%s&ico=scs&msg=scs',$lastID));

  } 

$form->setDefaults($program->programdata);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>