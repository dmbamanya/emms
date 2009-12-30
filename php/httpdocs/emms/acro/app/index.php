<?php
/*
 * Set cookie's secure flag if using ssl
 */
$_SERVER['HTTPS'] ? session_set_cookie_params(0,'/','',true,true) :  session_set_cookie_params(0,'/','',false,true); 

error_reporting(E_ALL);
include_once 'includes/trace.debugger.php';
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once 'Auth.php';
require_once './includes/ST.LIB.login.inc';

WEBPAGE::START();

/*
 * checks user auth. status
 */
$auth = new Auth('DB', WEBPAGE::$auth_options, 'loginFunction');
$auth->setFailedLoginCallback('loginFailedCallback');
$auth->setLoginCallback('loginCallback');
$auth->start();

/*
 * blocks session hacking from other emms modules like mod_sponsorship
 */
if ($_SESSION['_authsession']['data']['module'] != 'main' )
{
  $message = 'loggedOut';
  $auth->logout();
  loginFunction();
  exit;
} 

/*
 * blocks cross site hacking
 */
if (strpos('.'.$_SESSION['_authsession']['data']['url'], rtrim(WEBPAGE::$url,'/')) != 1)
{
  $message = 'loggedOut';
  $auth->logout();
  loginFunction();  
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
 * process logout request
 */
if (!empty($_REQUEST['logout']))
{
  $message = $_REQUEST['msg'] ? $_REQUEST['msg'] : 'loggedOut';
  $lang = WEBPAGE::$lang;
  $auth->logout();
  WEBPAGE::redirect(sprintf('index.php?msg=%s&lang=%s',$message,$lang));
  exit;
}

/*
 * validates run mode
 */ 
switch (WEBPAGE::$runMode)
{
  case WEBPAGE::_RUN_MODE_NORMAL:
    break;
  case WEBPAGE::_RUN_MODE_DEBUG:
    if (WEBPAGE::$userID == 1)
    {
      break;
    } else {
      WEBPAGE::redirect('index.php?logout=1&msg=loggedOut.maintenance&lang='.WEBPAGE::$lang);
      exit;
    }
  case WEBPAGE::_RUN_MODE_OUTDATED:
    require 'includes/index.update.inc';
    exit;
  default : 
    WEBPAGE::redirect('index.php?logout=1&msg=loggedOut.maintenance&lang='.WEBPAGE::$lang);
    exit;  	
}

/*
 * validates user's permissions to execute script on request
 */
if (!WEBPAGE::validateFn(WEBPAGE::$scr_name))
{ 
  WEBPAGE::redirect('index.php?logout=1&msg=loggedOut&lang='.WEBPAGE::$lang);
  exit;
}

require './includes/index.inc';

?>  