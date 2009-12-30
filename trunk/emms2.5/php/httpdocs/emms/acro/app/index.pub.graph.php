<?php
error_reporting(E_ERROR);
include_once 'includes/trace.debugger.php';
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once 'Auth.php';
require_once 'class/graph.php';

WEBPAGE::START();

// check user auth. status
$auth = new Auth('DB', WEBPAGE::$auth_options, 'checkAccess');
$auth->start();

// to be here you need to provide a trusted scr_name... see checkAccess function
WEBPAGE::LOAD_SESSION();
WEBPAGE::$lang = $_GET['lang'] ? $_GET['lang'] : WEBPAGE::$conf['app']['default_language'];
WEBPAGE::load_gt();
WEBPAGE::load_fn();

require './includes/'.$_GET['scr_name'].'.inc' ;

function checkAccess()
{ 
  (WEBPAGE::$fn['mask'][$_GET['scr_name']] == 255) ? '' : exit;
}

?>

