<?php
/*
 * Set cookie's secure flag if using ssl
 */
$_SERVER['HTTPS'] ? session_set_cookie_params(0,'/','',true,true) :  session_set_cookie_params(0,'/','',false,true);

error_reporting(E_ERROR);
set_include_path(get_include_path() . PATH_SEPARATOR . '../'); 
require_once './class/webpage.php';
require_once '../app/class/sql.php';
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
 * blocks session hacking from other emms modules
 */
if ($_SESSION['_authsession']['data']['module'] != 'mod_sponsorship' )
{
  $message = 'loggedOut';
  $auth->logout();
  loginFunction();
  exit;
}

/*
 * blocks cross site hacking
 */
if (WEBPAGE::$url.'index.php' != $_SESSION['_authsession']['data']['url'])
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
  WEBPAGE::redirect(sprintf('index.php?msg=%s&lang=%s&partner=%s',$message,$lang,WEBPAGE::$partner));
  exit;
}

// check run mode
/*
switch (WEBPAGE::$runMode) {
  case 'normal':
    break;
  default :
    $message = 'loggedOut.maintenance';
    $auth->logout();
    loginFunction();
    exit;
  }
*/

/*
 *  check permissions here ....
 */
if (!$_SESSION['_authsession']['data']['active'])
{
  $message = 'loggedOut';
  $auth->logout();
  loginFunction();
  exit;
}

WEBPAGE::$dbh->query(sprintf("update tblSponsorsLog set last_hit_date = NOW() where id = '%s'",$_SESSION['_authsession']['data']['session_id']));

require './class/gettext.php';
GETTEXT::START();

require './includes/index.inc';

?>