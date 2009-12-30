<?php
require_once 'HTML/Template/ITX.php';
require_once '../app/class/TTFButton.php';

if((WEBPAGE::$partner)&&( file_exists(sprintf(WEBPAGE::_APP_CUSTOM_TPL,WEBPAGE::$paths['3'],WEBPAGE::$paths['2']).'/mod_sponsorship/'.WEBPAGE::$partner.'/'.WEBPAGE::$scr_name.'.tpl')))
{
  $tpl = new HTML_Template_ITX(sprintf(WEBPAGE::_APP_CUSTOM_TPL,WEBPAGE::$paths['3'],WEBPAGE::$paths['2']).'/mod_sponsorship/'.WEBPAGE::$partner);
} else {
  $tpl = new HTML_Template_ITX('./templates');
}
if($_REQUEST['popup'])
{
  $tpl->loadTemplateFile('index.popup.tpl');
} else {
  $tpl->loadTemplateFile('index.tpl');
}

/*
 * root template parser
 */
$tpl->setCurrentBlock("html") ;
$tpl->setVariable('html_title_client_name',   strip_tags(WEBPAGE::$conf['app']['client_name']));
$tpl->setVariable('client_name',              WEBPAGE::$conf['app']['client_name']);
$tpl->setVariable('theme',                    WEBPAGE::$themePath);
$tpl->setVariable('sponsorship_program_name', WEBPAGE::$gt['sponsorship_program_name']);
$tpl->setVariable('screenWidth',              WEBPAGE::$screenWidth);
$tpl->setVariable('greetings',                sprintf(WEBPAGE::$gt['welcome'],$_SESSION['_authsession']['username'],date(WEBPAGE::$gt['date_format'])));

$tpl->setVariable('sponsor_name',             $_SESSION['_authsession']['data']['sponsor']);
$tpl->setVariable('sponsor_contact',          sprintf('%s %s %s',$_SESSION['_authsession']['data']['first'],$_SESSION['_authsession']['data']['middle'],$_SESSION['_authsession']['data']['last']));

if (WEBPAGE::$gt[$_GET['msg']])
{
  $tpl->addBlockfile('message','message','ST.SCR.message.tpl');
  $tpl->setCurrentBlock("message") ;
  $tpl->setVariable('ico',sprintf('%s/icons/%s.png',WEBPAGE::$themePath,$_GET['ico']));
  $tpl->setVariable('msg',WEBPAGE::$gt[$_GET['msg']]);
  $tpl->parseCurrentBlock("message") ;
}

/*
 * content parser
 */
$tpl->setVariable('content_title',WEBPAGE::$gt[WEBPAGE::$scr_name]);
$tpl->addBlockfile('content','content',sprintf("%s.%s",WEBPAGE::$scr_name,'tpl'));
$tpl->setCurrentBlock("content") ;

if ( file_exists(sprintf("./includes/%s.%s",WEBPAGE::$scr_name,'inc') ))
{
  include sprintf("./includes/%s.%s",WEBPAGE::$scr_name,'inc');
  } else {
  $tpl->addBlockfile('message','message','ST.SCR.message.tpl');
  $tpl->setCurrentBlock("message") ;
  $tpl->setVariable('ico',sprintf('%s/icons/%s.png',WEBPAGE::$themePath,'inf'));
  $tpl->setVariable('msg',WEBPAGE::$gt['inf.na']);
  $tpl->parseCurrentBlock("message") ;
}

if ( WEBPAGE::$scr_name != WEBPAGE::$conf['mod_sponsorship']['default_homepage'] )
{
  if ( $show_gohome )
  {
    $tpl->setVariable('gohome_ico', sprintf("<a href='index.php?scr_name=%s'><img border=0 src='%s/icons/48x48/home.png'></a>",WEBPAGE::$conf['mod_sponsorship']['default_homepage'],WEBPAGE::$themePath));
    $tpl->setVariable('gohome_txt', sprintf("<a href='index.php?scr_name=%s'>%s</a>",WEBPAGE::$conf['mod_sponsorship']['default_homepage'],WEBPAGE::$gt['quick_jumps']));
  }
}

$tpl->parseCurrentBlock("content") ;
$tpl->parseCurrentBlock("html") ;
$tpl->show();

?>