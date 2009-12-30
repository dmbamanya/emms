<?php
$show_gohome = true;

require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/sponsor.php';

$sponsor = new SPONSOR(WEBPAGE::$userID);
$btn     = new TTFButton(WEBPAGE::$theme);

$form    = new HTML_QuickForm('userForm','POST');

$form->addElement('static',   'submit',   '', $btn->imgsubmit('userForm',WEBPAGE::$gt['submit']));
$form->addElement('static',   'reset',    '', $btn->imgreset('userForm',WEBPAGE::$gt['reset']));
$form->addElement('static',   'cancel',   '', $btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));

$form->addElement('hidden',   'scr_name', $_REQUEST['scr_name']);
$form->addElement('hidden',   'id',       WEBPAGE::$userID);

$form->addElement('password', 'password', WEBPAGE::$gt['tblUsers.password'], 'class=large');
$form->addElement('password', 'verify',   WEBPAGE::$gt['verify'],            'class=large');

/*
 * register user-defined validation rules
 */
$form->registerRule('checkverify', 'callback', 'checkverify');

/*
 *  Add validation rules
 */
$form->addRule('password', sprintf(WEBPAGE::$gt['rangelength'],6,16),'required');
$form->addRule('password', sprintf(WEBPAGE::$gt['rangelength'],6,16),'rangelength', array(6,16));
$form->addRule('verify',   WEBPAGE::$gt['verifyPass'], 'required');
$form->addRule('verify',   WEBPAGE::$gt['verifyPass'], 'checkverify', true);

$form->freeze($sponsor->frostdata);

if ($form->validate())
{
  $form->freeze();
  $values = $form->_submitValues;
  $sponsor->changePassword(crypt($values['password'], md5($values['password'])));
  WEBPAGE::redirect(sprintf('index.php?scr_name=%s&ico=scs&msg=scs',WEBPAGE::$conf['mod_sponsorship']['default_homepage']));
}

$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

function checkverify($verify)
{
  if ($verify != $_POST['password']) return false;
  return true;
}
?>