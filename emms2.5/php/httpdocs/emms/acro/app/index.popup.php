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
//require_once 'ST.LIB.login.inc';

WEBPAGE::START();

/*
 * checks user auth. status
 */
$auth = new Auth('DB', WEBPAGE::$auth_options, 'closePopup');
$auth->setFailedLoginCallback('closePopup');
$auth->setLoginCallback('closePopup');
$auth->start();

/*
 * blocks session hacking from other emms modules like mod_sponsorship
 */
if ($_SESSION['_authsession']['data']['module'] != 'main' )
{
  closePopup();
}

/*
 *  blocks cross site hacking
 */
if (strpos('.'.$_SESSION['_authsession']['data']['url'], rtrim(WEBPAGE::$url,'/')) != 1)
{
  closePopup();
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
  default :   closePopup();
}

/*
 * validates user's permissions to execute script on request
 */
if (!WEBPAGE::validateFn(WEBPAGE::$scr_name))
{ 
  closePopup();
}

require './includes/index.popup.inc';

function closePopup()
{
  echo "<body onLoad='window.close()'>";
  exit;
}

?>