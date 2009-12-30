<?php
require_once 'HTML/Template/ITX.php';
require_once 'class/TTFButton.php';

$tpl = new HTML_Template_ITX('./templates');
$tpl->loadTemplateFile('index.tpl');


//tabmenu template parser
for ( $i=0;$i<count(WEBPAGE::$tabmenu);$i++ )
{
  $tpl->setCurrentBlock("tabitem");
  $tpl->setVariable("URL", WEBPAGE::$tabmenu[$i]['URL']) ;
  $tpl->setVariable("TAB", WEBPAGE::$tabmenu[$i]['TAB']) ;
  $tpl->parseCurrentBlock("tabitem");
}
$tpl->setCurrentBlock("tabmenu") ;
$tpl->parseCurrentBlock("tabmenu") ;

//current path template parser


//root template parser
$tpl->setCurrentBlock("html") ;
$tpl->setVariable('html_title_client_name', strip_tags(WEBPAGE::$conf['app']['client_name']));
$tpl->setVariable('client_name',            WEBPAGE::$conf['app']['client_name']);
$tpl->setVariable('theme',                  WEBPAGE::$themePath);
$tpl->setVariable('screenWidth',            WEBPAGE::$screenWidth);
$tpl->setVariable('greetings',              sprintf(WEBPAGE::$gt['welcome'],WEBPAGE::$userName,date(WEBPAGE::$gt['date_format'])));
$tpl->setVariable('navtree',                WEBPAGE::$navtree);
$tpl->setVariable('path_caption',           WEBPAGE::$fn['path'][WEBPAGE::$scr_name] ? WEBPAGE::$gt['path'] : ($_REQUEST['ref'] ? WEBPAGE::$gt['path'] : '') );
$tpl->setVariable('path',                   WEBPAGE::$fn['path'][WEBPAGE::$scr_name] ? WEBPAGE::$fn['path'][WEBPAGE::$scr_name] : WEBPAGE::$fn['path'][$_REQUEST['ref']]);

if (WEBPAGE::$gt[$_GET['msg']])
{
  $tpl->addBlockfile('message','message','ST.SCR.message.tpl');
  $tpl->setCurrentBlock("message") ;
  $tpl->setVariable('ico',sprintf('%s/icons/%s.png',WEBPAGE::$themePath,$_GET['ico']));
  $tpl->setVariable('msg',WEBPAGE::$gt[$_GET['msg']]);
  $tpl->parseCurrentBlock("message") ;
}

//content parser
$tpl->setVariable('content_title',WEBPAGE::$gt[WEBPAGE::$scr_name]);
$tpl->addBlockfile('content','content',sprintf("%s.%s",WEBPAGE::$scr_name,'tpl'));
$tpl->setCurrentBlock("content");

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
$tpl->parseCurrentBlock("content") ;


$tpl->parseCurrentBlock("html") ;

$tpl->show();
?>