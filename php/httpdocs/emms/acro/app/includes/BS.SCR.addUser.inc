<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/user.php';

$user = new USER($_REQUEST['id']);
$btn  = new TTFButton(WEBPAGE::$theme);

foreach(WEBPAGE::$fn['qjump'] as $key => $val)
{
  $qjump[$key] = WEBPAGE::$gt[$key];
  if(!is_array(WEBPAGE::$fn['popup'][$key])) { $home[$key] = WEBPAGE::$gt[$key]; }
}

$form = new HTML_QuickForm('userForm','POST');

$form->addElement('static',      'submit', '', $btn->imgsubmit('userForm',WEBPAGE::$gt['submit']));
$form->addElement('static',      'reset',  '', $btn->imgreset('userForm',WEBPAGE::$gt['reset']));
$form->addElement('static',      'cancel', '', $btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));

$form->addElement('hidden',      'scr_name',   $_REQUEST['scr_name']);
$form->addElement('hidden',      'id',         $_REQUEST['id']);
$form->addElement('hidden',      'oldpassword');

$form->addElement('password',    'password',   WEBPAGE::$gt['tblUsers.password'],  'class=large');
$form->addElement('password',    'verify',     WEBPAGE::$gt['verify'],             'class=large');

$form->addElement('text',        'username',   WEBPAGE::$gt['tblUsers.username'],  'class=large');
$form->addElement('text',        'first',      WEBPAGE::$gt['tblUsers.first'],     'class=large');
$form->addElement('text',        'middle',     WEBPAGE::$gt['tblUsers.middle'],    'class=large');
$form->addElement('text',        'last',       WEBPAGE::$gt['tblUsers.last'],      'class=large');
$form->addElement('text',        'birthdate',  WEBPAGE::$gt['tblUsers.birthdate'], 'class=large');
$form->addElement('text',        'code',       WEBPAGE::$gt['tblUsers.code'],      'class=large');
$form->addElement('text',        'email',      WEBPAGE::$gt['tblUsers.email'],     'class=large');

$form->addElement('textarea',    'memo',       WEBPAGE::$gt['tblUsers.memo'],      'class=large');

$form->addElement('select',      'zone_id',    WEBPAGE::$gt['tblUsers.zone_id'],   $user->zones());
$form->addElement('select',      'super_id',   WEBPAGE::$gt['tblUsers.super_id'],  $user->supervisors());
$form->addElement('select',      'home',       WEBPAGE::$gt['tblUsers.home'],      $home,  'class=large');
$form->addElement('select',      'qjump',      WEBPAGE::$gt['tblQjumps.qjump'],    $qjump, 'multiple size="5" class=large');

$form->addElement('advcheckbox', 'active',     WEBPAGE::$gt['tblUsers.active'],    WEBPAGE::$gt['tblUsers.active.1'],'class=checkbox');
$form->addElement('advcheckbox', 'staff',      WEBPAGE::$gt['tblUsers.staff'],     WEBPAGE::$gt['tblUsers.staff.1'], 'class=checkbox');

foreach ($user->roles() as $key => $value)
{
  $roles_advcheckbox[] = $form->createElement('advcheckbox',$key,null,$value['label'],'class=checkbox',$value['code']);
}
$form->addGroup($roles_advcheckbox, 'roles', WEBPAGE::$gt['tblUsers.access_code'],'<br>');

$gender_radio[] = $form->createElement('radio',null,null,WEBPAGE::$gt['tblUsers.gender.M'],'M', 'class=radio');
$gender_radio[] = $form->createElement('radio',null,null,WEBPAGE::$gt['tblUsers.gender.F'],'F', 'class=radio');
$form->addGroup($gender_radio, 'gender', WEBPAGE::$gt['tblUsers.gender'],'<br />');

$form->registerRule('checkusername', 'callback', 'checkusername');
$form->registerRule('checkqjump',    'callback', 'checkqjump');
$form->registerRule('rolesconflict', 'callback', 'rolesconflict');

//add rules
$form->addRule('qjump',     sprintf(WEBPAGE::$gt['maxselections'],16), 'checkqjump',    true);
$form->addRule('qjump',     WEBPAGE::$gt['rolesconflict'],             'rolesconflict', true);
$form->addRule('home',      WEBPAGE::$gt['rolesconflict'],             'rolesconflict', true);
$form->addRule('username',  WEBPAGE::$gt['usernameExists'],            'checkusername', true);
$form->addRule('username',  sprintf(WEBPAGE::$gt['rangelength'],4,12), 'required');
$form->addRule('username',  sprintf(WEBPAGE::$gt['rangelength'],4,12), 'rangelength',   array(4,12));
$form->addRule('username',  WEBPAGE::$gt['lettersonly'],               'lettersonly');
$form->addRule('password',  sprintf(WEBPAGE::$gt['rangelength'],6,16), 'required');
$form->addRule('password',  sprintf(WEBPAGE::$gt['rangelength'],6,16), 'rangelength',   array(6,16));
$form->addRule('verify',    sprintf(WEBPAGE::$gt['rangelength'],6,16), 'required');
$form->addRule('verify',    WEBPAGE::$gt['verifyPass'],                'regex',         sprintf("(^(%s{1})$)",$_POST['password']));
$form->addRule('first',     sprintf(WEBPAGE::$gt['rangelength'],1,16), 'required');
$form->addRule('first',     sprintf(WEBPAGE::$gt['rangelength'],1,16), 'rangelength',   array(1,16));
$form->addRule('first',     WEBPAGE::$gt['lettersonly'],               'lettersonly');
$form->addRule('middle',    sprintf(WEBPAGE::$gt['rangelength'],1,16), 'rangelength',   array(1,16));
$form->addRule('middle',    WEBPAGE::$gt['lettersonly'],               'regex',         '(^[a-zA-Z\. ]*$)');
$form->addRule('last',      sprintf(WEBPAGE::$gt['rangelength'],1,32), 'required');
$form->addRule('last',      sprintf(WEBPAGE::$gt['rangelength'],1,32), 'rangelength',   array(1,32));
$form->addRule('last',      WEBPAGE::$gt['lettersonly'],               'regex',         '(^[a-zA-Z\. ]*$)');
$form->addRule('birthdate', 'yyyy-mm-dd',                              'required');
$form->addRule('birthdate', 'yyyy-mm-dd',                              'regex',         '(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('code',      WEBPAGE::$gt['required'],                  'required');
$form->addRule('code',      WEBPAGE::$conf['app']['regex_govID_tip'],  'regex',         WEBPAGE::$conf['app']['regex_govID']);
$form->addRule('email',     WEBPAGE::$gt['emailorblank'],              'emailorblank');
$form->addRule('gender',    WEBPAGE::$gt['required'],                  'required');
$form->addRule('memo',      sprintf(WEBPAGE::$gt['maxlength'],255),    'maxlength',      255);

$form->freeze($user->frostdata);

if ($form->validate())
{
  $form->freeze();
  $values                  = $form->_submitValues;
  $values['editor_date']   = date('Y-m-d');
  $values['creator_date']  = $user->userdata['creator_date'] ? $user->userdata['creator_date'] : $values['editor_date'];
  $values['editor_id']     = WEBPAGE::$userID;
  $values['creator_id']    = $user->userdata['creator_id']   ? $user->userdata['creator_id']   : $values['editor_id'];
  $values['access_code']   = array_sum($values['roles']);
  if ($values['password'] != $values['oldpassword'])
  {
    $values['password'] = crypt($values['password'], md5($values['password']));
  } else {
    unset ($values['password']);
  }
  $user->userdata['id'] = SQL::insert('tblUsers',$values,'id');
  $user->setQjumps($values['qjump']);
  WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewUser&id=%s&ico=scs&msg=scs',$user->userdata['id']));
}

$form->setDefaults($user->userdata);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

function checkusername($username)
{
  $check = WEBPAGE::$dbh->getAll(sprintf("select id,username from tblUsers where username = '%s' and id != '%s'",$username,$_REQUEST['id']));
  if (count($check)) { return false; } else { return true; }
}

function checkqjump($qjump)
{
  if (count($qjump) > 16) { return false; } else { return true; }
}

function rolesconflict($fn)
{
  $fn = is_array($fn) ? $fn : array('0'=>$fn);
  foreach($fn as $f)
  {
    if (!( intval(array_sum($_REQUEST['roles'])) & intval(WEBPAGE::$fn['mask'][$f]) )) return false;
  }
  return true;
}
?>