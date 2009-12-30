<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/user.php';
require_once 'Date.php';

$user = new USER(WEBPAGE::$userID);
$btn  = new TTFButton(WEBPAGE::$theme);

foreach(WEBPAGE::$fn['qjump'] as $key => $val)
{
  if(WEBPAGE::validateFn($key))
  {
    $qjump[$key] = WEBPAGE::$gt[$key];
    if(!is_array(WEBPAGE::$fn['popup'][$key])) { $home[$key] = WEBPAGE::$gt[$key]; }
  }
}

$form = new HTML_QuickForm('userForm','POST');

$form->addElement('static',    'submit',       '', $btn->imgsubmit('userForm',WEBPAGE::$gt['submit']));
$form->addElement('static',    'reset',        '', $btn->imgreset('userForm',WEBPAGE::$gt['reset']));

$form->addElement('hidden',    'scr_name',         $_REQUEST['scr_name']);
$form->addElement('hidden',    'id',               WEBPAGE::$userID);

$form->addElement('password',  'old_password',     WEBPAGE::$gt['old_password'],       'class=large');
$form->addElement('password',  'password',         WEBPAGE::$gt['new_password'],       'class=large');
$form->addElement('password',  'verify',           WEBPAGE::$gt['verify_password'],    'class=large');

$form->addElement('text',      'first',            WEBPAGE::$gt['tblUsers.first'],     'class=large');
$form->addElement('text',      'middle',           WEBPAGE::$gt['tblUsers.middle'],    'class=large');
$form->addElement('text',      'last',             WEBPAGE::$gt['tblUsers.last'],      'class=large');
$form->addElement('text',      'birthdate',        WEBPAGE::$gt['tblUsers.birthdate'], 'class=large');
$form->addElement('text',      'code',             WEBPAGE::$gt['tblUsers.code'],      'class=large');
$form->addElement('text',      'email',            WEBPAGE::$gt['tblUsers.email'],     'class=large');
$form->addElement('select',    'qjump',            WEBPAGE::$gt['tblQjumps.qjump'],    $qjump, 'multiple size="4" class=large');
$form->addElement('select',    'home',             WEBPAGE::$gt['tblUsers.home'],      $home,  'size="4" class=large');
$form->addElement('select',    'gender',           WEBPAGE::$gt['tblUsers.gender'],    array('F'=>WEBPAGE::$gt['tblUsers.gender.F'],'M'=>WEBPAGE::$gt['tblUsers.gender.M']),  'size="2" class=large');

/*
 * register user-defined validation rules
 */
$form->registerRule('checkqjump', 	            'callback', 'checkqjump');
$form->registerRule('validatepasswordmetrics',      'callback', 'validatepasswordmetrics');
$form->registerRule('validatePasswordUsageHistory', 'callback', 'validatePasswordUsageHistory');
$form->registerRule('checkverify', 	            'callback', 'checkverify');
$form->registerRule('checkoldpassword',             'callback', 'checkoldpassword');

/*
 * Add validation rules
 */
if($_POST['old_password']||$_POST['password']||$_POST['verify'])
{
  $form->addRule('old_password', WEBPAGE::$gt['required'], 'required');
  $form->addRule('password',     WEBPAGE::$gt['required'], 'required');
  $form->addRule('verify',       WEBPAGE::$gt['required'], 'required');
}
$form->addRule('qjump', 	sprintf(WEBPAGE::$gt['maxselections'],16), 'checkqjump',           true);
$form->addRule('password', 	sprintf(WEBPAGE::$gt['pwd_metrics_tip'],   WEBPAGE::$conf['app']['pwd_min_length'],WEBPAGE::$conf['app']['pwd_max_length'],WEBPAGE::$conf['app']['pwd_min_uppercase'],WEBPAGE::$conf['app']['pwd_min_lowercase'],WEBPAGE::$conf['app']['pwd_min_number'],WEBPAGE::$conf['app']['pwd_min_special']),'validatepasswordmetrics', true);
$form->addRule('password', 	sprintf(WEBPAGE::$gt['pwd_history_tip'],   WEBPAGE::$conf['app']['pwd_diff_period'],WEBPAGE::$conf['app']['pwd_min_diff']),'validatePasswordUsageHistory', true);
$form->addRule('old_password', 	WEBPAGE::$gt['checkoldpassword'],          'checkoldpassword',     true);
$form->addRule('verify',        WEBPAGE::$gt['verifyPass'],                'checkverify',          true);
$form->addRule('first',         sprintf(WEBPAGE::$gt['rangelength'],1,16), 'required');
$form->addRule('first',         sprintf(WEBPAGE::$gt['rangelength'],1,16), 'rangelength',          array(1,16));
$form->addRule('first',         WEBPAGE::$gt['lettersonly'],               'lettersonly');
$form->addRule('middle',        sprintf(WEBPAGE::$gt['rangelength'],1,16), 'rangelength',          array(1,16));
$form->addRule('middle',        WEBPAGE::$gt['lettersonly'],               'regex',                '(^[a-zA-Z\. ]*$)');
$form->addRule('last',          sprintf(WEBPAGE::$gt['rangelength'],1,32), 'required');
$form->addRule('last',          sprintf(WEBPAGE::$gt['rangelength'],1,32), 'rangelength',          array(1,32));
$form->addRule('last',          WEBPAGE::$gt['lettersonly'],               'regex',                '(^[a-zA-Z\. ]*$)');
$form->addRule('birthdate',     'yyyy-mm-dd',                              'required');
$form->addRule('birthdate',     'yyyy-mm-dd',                              'regex',                '(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('code',          WEBPAGE::$gt['required'],                  'required');
$form->addRule('code',          WEBPAGE::$conf['app']['regex_govID_tip'],  'regex',                WEBPAGE::$conf['app']['regex_govID']);
$form->addRule('email',         WEBPAGE::$gt['emailorblank'],              'emailorblank');
$form->addRule('gender',        WEBPAGE::$gt['required'],                  'required');

$form->freeze($user->frostdata);

if ($form->validate())
{
  $form->freeze();

  $values = $form->_submitValues;
  $values['editor_date']         = date('Y-m-d');
  $values['editor_id']           = WEBPAGE::$userID;
  
  if ($values['password'] != '')
  {
    $values['password'] = crypt($values['password'], md5($values['password']));
    $fdate = new Date();
    $fdate->addSeconds(24*60*60*WEBPAGE::$conf['app']['pwd_force_period']);
    $values['force_next_pwd_date'] = $fdate->format('%Y-%m-%d');
  } else {
    unset ($values['password']);
  }

  $lastID = SQL::insert('tblUsers',$values,'id');
  $user->setQjumps($values['qjump']);

  WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewUser&id=%s&ico=scs&msg=scs',WEBPAGE::$userID));
  exit;
}

unset($user->userdata['password']);
unset($user->userdata['verify']);
$form->setDefaults($user->userdata);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

$tpl->setVariable('multi_select_tip', WEBPAGE::$gt['multi_select_tip']);
$tpl->setVariable('pwd_leave_blank',  WEBPAGE::$gt['pwd_leave_blank']);

/*
 *  Form field validation functions
 */

function checkqjump($qjump)
{  
  if (count($qjump) > 16) { return false; } else { return true; }
}

function checkverify($verify)
{  
  if ($verify != $_POST['password']) return false;
  return true;
}

function checkoldpassword($old_password)
{
  global $user;
  return $user->validatePassword($old_password);
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
