<?php
/*
 * Set cookie's secure flag if using ssl
 */
$_SERVER['HTTPS'] ? session_set_cookie_params(0,'/','',true,true) :  session_set_cookie_params(0,'/','',false,true);

error_reporting(E_ERROR);
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once 'Auth.php';
require_once 'class/graph.php';

WEBPAGE::START();

/*
 * checks user auth. status
 */
$auth = new Auth('DB', WEBPAGE::$auth_options, 'exitProcess');
$auth->setFailedLoginCallback('exitProcess');
$auth->setLoginCallback('exitProcess');
$auth->start();

/*
 * blocks session hacking from other emms modules like mod_sponsorship
 */
if ($_SESSION['_authsession']['data']['module'] != 'main' )
{
  exit;
}

/*
 *  blocks cross site hacking
 */
if (strpos('.'.$_SESSION['_authsession']['data']['url'], rtrim(WEBPAGE::$url,'/')) != 1)
{
  exit;
}

/*
 * only valid username/password or a valid session already open get here
 * so now we load session vars, localized texts, fn masks, paths, etc.
 */
WEBPAGE::LOAD_SESSION();
WEBPAGE::load_gt();
WEBPAGE::load_fn();

/*
 * validates run mode
 */
switch (WEBPAGE::$runMode)
{
  case WEBPAGE::_RUN_MODE_NORMAL: break;
  case WEBPAGE::_RUN_MODE_DEBUG: if (WEBPAGE::$userID == 1) {  break; } else { exit; }
  default : exit;
}

/*
 * validates user's permissions to execute script on request
 */
if (!((WEBPAGE::$fn['mask'][WEBPAGE::$scr_name] == '255')||( WEBPAGE::validateFn(WEBPAGE::$scr_name) )))
{
  exit;
}

if ( file_exists('./includes/'.WEBPAGE::$scr_name.'.inc'))
{
  require './includes/'.WEBPAGE::$scr_name.'.inc';
} else {
  exit;
}

function exitProcess()
{
  exit;
}

?>
