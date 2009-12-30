<?php
 /*
 * set app language & load localized texts
 */
WEBPAGE::$lang = isset($_POST['lang']) ? $_POST['lang'] : ( isset($_GET['lang']) ? $_GET['lang'] : WEBPAGE::$conf['mod_sponsorship']['default_language'] );
WEBPAGE::load_gt();

require_once 'HTML/Template/ITX.php';
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once './class/TTFButton.php';

$btn = new TTFButton() ;

$form = new HTML_QuickForm('loginForm','POST');

$form->addElement('static',    'submit',      '', $btn->imgsubmit('loginForm',WEBPAGE::$gt['submit']));
$form->addElement('text',      'username',        WEBPAGE::$gt['tblUsers.username'], 'class=qform');
$form->addElement('password',  'password',        WEBPAGE::$gt['tblUsers.password'], 'class=qform');
$form->addElement('hidden',    'screenWidth', '');
$form->addElement('hidden',    'lang',            WEBPAGE::$lang);
$form->addElement('hidden',    'partner',         $_REQUEST['partner'] ? $_REQUEST['partner'] : '');


$form->addRule('username',WEBPAGE::$gt['MSG.WRN.078'],'required');

$tpl = new HTML_Template_ITX('./templates');
$tpl->loadTemplateFile('login.tpl');

if((WEBPAGE::$partner)&&( file_exists(sprintf(WEBPAGE::_APP_CUSTOM_TPL,WEBPAGE::$paths['3'],WEBPAGE::$paths['2']).'/mod_sponsorship/'.WEBPAGE::$partner.'/login.tpl')))
{
  $tpl = new HTML_Template_ITX(sprintf(WEBPAGE::_APP_CUSTOM_TPL,WEBPAGE::$paths['3'],WEBPAGE::$paths['2']).'/mod_sponsorship/'.WEBPAGE::$partner);
} else {
  $tpl = new HTML_Template_ITX('./templates');
}
$tpl->loadTemplateFile('login.tpl');

$tpl->setVariable('html_title_client_name',   strip_tags(WEBPAGE::$conf['app']['client_name']));
$tpl->setVariable('client_name',              WEBPAGE::$conf['app']['client_name']);
$tpl->setVariable('sponsorship_program_name', WEBPAGE::$gt['sponsorship_program_name']);
$tpl->setVariable('partner',                  $_REQUEST['partner']);
$tpl->setVariable('message',                  WEBPAGE::$gt[$message]);
$tpl->setVariable('copyright',                WEBPAGE::$gt['copyright']);
$tpl->setVariable('contact',                  WEBPAGE::$gt['app_contact_info']);

$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(_REQUIRED_FIELD);
$renderer->setErrorTemplate(_FIELD_ERROR);

$form->accept($renderer);
$tpl->show();
?>