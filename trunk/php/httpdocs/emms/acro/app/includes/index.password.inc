<?php
require_once 'HTML/Template/ITX.php';
require_once 'class/TTFButton.php';
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/user.php';
require_once 'Date.php';

$user = new USER(WEBPAGE::$userID);
$btn  = new TTFButton(WEBPAGE::$theme);

$tpl = new HTML_Template_ITX('./templates');
$tpl->loadTemplateFile('index.password.tpl');

$form = new HTML_QuickForm('userForm','POST');

$form->addElement('static',    'submit',       '', $btn->imgsubmit('userForm',WEBPAGE::$gt['submit']));
$form->addElement('password',  'old_password',     WEBPAGE::$gt['old_password']);
$form->addElement('password',  'password',         WEBPAGE::$gt['new_password']);
$form->addElement('password',  'verify',           WEBPAGE::$gt['verify_password']);

/*
 * register user-defined validation rules
 */
$form->registerRule('validatepasswordmetrics',      'callback', 'validatepasswordmetrics');
$form->registerRule('validatePasswordUsageHistory', 'callback', 'validatePasswordUsageHistory');
$form->registerRule('checkverify', 	            'callback', 'checkverify');
$form->registerRule('checkoldpassword',             'callback', 'checkoldpassword');

/*
 * Add validation rules
 */
$form->addRule('old_password', WEBPAGE::$gt['required'], 'required');
$form->addRule('old_password', WEBPAGE::$gt['checkoldpassword'],          'checkoldpassword',     true);
$form->addRule('password',     WEBPAGE::$gt['required'], 'required');
$form->addRule('password',     sprintf(WEBPAGE::$gt['pwd_metrics_tip'],   WEBPAGE::$conf['app']['pwd_min_length'],WEBPAGE::$conf['app']['pwd_max_length'],WEBPAGE::$conf['app']['pwd_min_uppercase'],WEBPAGE::$conf['app']['pwd_min_lowercase'],WEBPAGE::$conf['app']['pwd_min_number'],WEBPAGE::$conf['app']['pwd_min_special']),'validatepasswordmetrics', true);
$form->addRule('password',     sprintf(WEBPAGE::$gt['pwd_history_tip'],   WEBPAGE::$conf['app']['pwd_diff_period'],WEBPAGE::$conf['app']['pwd_min_diff']),'validatePasswordUsageHistory', true);
$form->addRule('verify',       WEBPAGE::$gt['required'], 'required');
$form->addRule('verify',       WEBPAGE::$gt['verifyPass'],                'checkverify',          true);

if ($form->validate())
{
  $form->freeze();
  $values = $form->_submitValues;
  $values['id'] = WEBPAGE::$userID;
  $values['password'] = crypt($values['password'], md5($values['password']));
  $fdate = new Date();
  $fdate->addSeconds(24*60*60*WEBPAGE::$conf['app']['pwd_force_period']);
  $values['force_next_pwd_date'] = $fdate->format('%Y-%m-%d'); 
  SQL::insert('tblUsers',$values,'id');
  WEBPAGE::$userAccessCode = $user->userdata['access_code'];
  WEBPAGE::redirect('index.php');
  exit;
}

//$form->setDefaults($user->userdata);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

$tpl->setVariable('html_title_client_name', strip_tags(WEBPAGE::$conf['app']['client_name']));
$tpl->setVariable('client_name',            WEBPAGE::$conf['app']['client_name']);
$tpl->setVariable('theme',                  WEBPAGE::$themePath);
$tpl->setVariable('welcome',                $user->userdata['first'].' '.$user->userdata['middle'].' '.$user->userdata['last']);
$tpl->setVariable('expired',                sprintf(WEBPAGE::$gt['pwd_expired']));
$tpl->setVariable('memorize',               sprintf(WEBPAGE::$gt['pwd_memorize']));

$tpl->show();

/*
 *  Form field validation functions
 */

function checkoldpassword($old_password)
{
  global $user;
  return $user->validatePassword($old_password);
}

function checkverify($verify)
{
  if ($verify != $_POST['password']) return false;
  return true;
}

function validatepasswordmetrics($password)
{
  return USER::validatePasswordMetrics($password);
}

function validatePasswordUsageHistory($password)
{
  global $user;
  return $user->validatePasswordUsageHistory($_POST['old_password'], $password);
}
?>


