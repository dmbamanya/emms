<?php
$show_gohome = true;

require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/sponsor.php';

$sponsor = new SPONSOR(WEBPAGE::$userID);
$btn     = new TTFButton(WEBPAGE::$theme);

$form    = new HTML_QuickForm('userForm','POST');

$form->addElement('static', 'submit', 	  '', $btn->imgsubmit('userForm',WEBPAGE::$gt['submit']));
$form->addElement('static', 'reset', 	    '', $btn->imgreset('userForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 'cancel', 	  '', $btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));

$form->addElement('hidden', 'scr_name',   $_REQUEST['scr_name']);
$form->addElement('hidden', 'id',			    WEBPAGE::$userID);

$form->addElement('text',		'username',		WEBPAGE::$gt['tblUsers.username'],   'class=large');
$form->addElement('text',		'sponsor',	  WEBPAGE::$gt['tblSponsors.sponsor'], 'class=large');
$form->addElement('text',		'first',		  WEBPAGE::$gt['tblUsers.first'],      'class=large');
$form->addElement('text',		'middle',		  WEBPAGE::$gt['tblUsers.middle'],     'class=large');
$form->addElement('text',		'last',			  WEBPAGE::$gt['tblUsers.last'],       'class=large');
$form->addElement('text',		'email',		  WEBPAGE::$gt['tblUsers.email'],      'class=large');

$form->addElement('textarea',	'memo',			WEBPAGE::$gt['tblUsers.memo']);

//add rules
$form->addRule('username',	sprintf(WEBPAGE::$gt['rangelength'],4,128),'required');
$form->addRule('username',	sprintf(WEBPAGE::$gt['rangelength'],4,128),'rangelength', array(4,128));
$form->addRule('username',	WEBPAGE::$gt['emailorblank'],'emailorblank');
$form->addRule('sponsor',	  sprintf(WEBPAGE::$gt['rangelength'],1,255),'required');
$form->addRule('sponsor',	  sprintf(WEBPAGE::$gt['rangelength'],1,255),'rangelength', array(1,255));
//$form->addRule('sponsor',	  WEBPAGE::$gt['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('first',		  sprintf(WEBPAGE::$gt['rangelength'],1,128),'required');
$form->addRule('first',		  sprintf(WEBPAGE::$gt['rangelength'],1,128),'rangelength', array(1,128));
//$form->addRule('first',		  WEBPAGE::$gt['lettersonly'],'lettersonly');
$form->addRule('middle',	  sprintf(WEBPAGE::$gt['rangelength'],1,128),'rangelength', array(1,128));
//$form->addRule('middle',	  WEBPAGE::$gt['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('last',		  sprintf(WEBPAGE::$gt['rangelength'],1,128),'required');
$form->addRule('last',		  sprintf(WEBPAGE::$gt['rangelength'],1,128),'rangelength', array(1,128));
//$form->addRule('last',		  WEBPAGE::$gt['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('email',		  WEBPAGE::$gt['required'],'required');
$form->addRule('email',		  WEBPAGE::$gt['emailorblank'],'emailorblank');
$form->addRule('memo',		  sprintf(WEBPAGE::$gt['maxlength'],255),'maxlength', 255);

$form->freeze($sponsor->frostdata);

if ($form->validate()) {

  $form->freeze();

  $values = $form->_submitValues;

  $sponsor->data['sponsor']     = $values['sponsor'];
  $sponsor->data['first']       = $values['first'];
  $sponsor->data['middle']      = $values['middle'];
  $sponsor->data['last']        = $values['last'];
  $sponsor->data['email']       = $values['email'];
  $sponsor->data['memo']        = $values['memo'];
//  $sponsor->data['editor_id']   = $sponsor->data['editor_id'];
//  $sponsor->data['editor_date'] = date('Y-m-d');

/*
  if ($values['password'] != $values['oldpassword']) {
  	$values['password'] =	crypt($values['password'], md5($values['password']));
  	} else {
  	unset ($values['password']);
  	}
*/
  $sponsor->update();
  
  $_SESSION['_authsession']['data']['first']   = $sponsor->data['first'];
  $_SESSION['_authsession']['data']['middle']  = $sponsor->data['middle'];
  $_SESSION['_authsession']['data']['last']    = $sponsor->data['last'];
  $_SESSION['_authsession']['data']['sponsor'] = $sponsor->data['sponsor'];
  
  WEBPAGE::redirect(sprintf('index.php?scr_name=%s&ico=scs&msg=scs',WEBPAGE::$conf['mod_sponsorship']['default_homepage']));

  }

$tpl->setVariable('contact_info', WEBPAGE::$gt['contact_info']);
$form->setDefaults($sponsor->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

?>