<?php
// if auto_cron = 1 este script no se puede correr, ademas de que el script
// que realmente actualiza (index.cron.php) no le hara caso

if (WEBPAGE::$conf['app']['auto_cron'] == '1')
{ 
  WEBPAGE::redirect('index.php?logout=1&msg=loggedOut.maintenance&lang='.WEBPAGE::$lang);
  exit;
}

require_once 'HTML/Template/ITX.php';
require_once 'class/TTFButton.php';

$btn = new TTFButton() ;

$tpl = new HTML_Template_ITX('./templates');
$tpl->loadTemplateFile('index.update.tpl');

$tpl->setVariable('html_title_client_name',	strip_tags(WEBPAGE::$conf['app']['client_name']));
$tpl->setVariable('client_name',	        WEBPAGE::$conf['app']['client_name']);
$tpl->setVariable('db_outdated',		WEBPAGE::$gt['db_outdated']);
$tpl->setVariable('db_outdated_inf',		sprintf(WEBPAGE::$gt['db_outdated_inf'],WEBPAGE::$runDate));
$tpl->setVariable('db_outdated_wrn',		WEBPAGE::$gt['db_outdated_wrn']);
$tpl->setVariable('copyright',		        WEBPAGE::$gt['copyright']);
$tpl->setVariable('update_button',		$btn->imglink('index.cron.php',WEBPAGE::$gt['submit']));

$tpl->setVariable('emms_version',		file_get_contents('../version'));

$tpl->show();
?>