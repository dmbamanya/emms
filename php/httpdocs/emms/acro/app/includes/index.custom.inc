<?php
require_once 'HTML/Template/ITX.php';
require_once 'class/TTFButton.php';

$tpl = new HTML_Template_ITX(sprintf(WEBPAGE::_APP_CUSTOM_TPL,WEBPAGE::$paths['3'],WEBPAGE::$paths['2']));

if ( file_exists(sprintf(WEBPAGE::_APP_CUSTOM_TPL,WEBPAGE::$paths['3'],WEBPAGE::$paths['2']).'/'.WEBPAGE::$scr_name.'.tpl') )
{
  $tpl->loadTemplateFile(WEBPAGE::$scr_name.'.tpl');
}

if ( file_exists(sprintf(WEBPAGE::_APP_CUSTOM_INC,WEBPAGE::$paths['3'],WEBPAGE::$paths['2']).'/'.WEBPAGE::$scr_name.'.inc') )
{
  include sprintf(WEBPAGE::_APP_CUSTOM_INC,WEBPAGE::$paths['3'],WEBPAGE::$paths['2']).'/'.WEBPAGE::$scr_name.'.inc';
}

$tpl->show();

?>
