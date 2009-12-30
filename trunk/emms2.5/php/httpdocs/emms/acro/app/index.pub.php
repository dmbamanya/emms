<?php
error_reporting(E_ERROR);
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once 'Auth.php';

WEBPAGE::START();
WEBPAGE::$lang = $_GET['lang'] ? $_GET['lang'] : WEBPAGE::$conf['app']['default_language'];
WEBPAGE::load_gt();
WEBPAGE::load_fn();

// check user auth. status
$auth = new Auth('DB', WEBPAGE::$auth_options, 'checkAccess');
$auth->start();

// to be here you need to provide a trusted scr_name... see checkAccess function
//WEBPAGE::LOAD_SESSION();

require './includes/index.pub.inc';

function checkAccess()
{ 
  (WEBPAGE::$fn['mask'][$_GET['scr_name']] == '255') ? '' : exit;
}

?>
