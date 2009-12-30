<?
require_once 'class/uprofile.php';

$roles = UPROFILE::getRoles($_SESSION['_authsession']['data']['access_code']);
$user_home = WEBPAGE::$dbh->getAssoc(sprintf('select id,home from tblUsers where id = "%s"',WEBPAGE::$userID));

$_REQUEST['scr_name'] = $user_home[WEBPAGE::$userID];

$tpl->setVariable('javascript',        "<SCRIPT type='text/javascript' src='CS.functions.js'></SCRIPT>");
$tpl->setVariable('welcome',           sprintf(WEBPAGE::$gt['welcome_home'], $_SESSION['_authsession']['data']['first']));
$tpl->setVariable('roles_caption',     WEBPAGE::$gt['tblUsers.access_code']);
$tpl->setVariable('roles',             implode(', ',$roles));
$tpl->setVariable('user_home_tip',     sprintf(WEBPAGE::$gt['user_home_tip'],WEBPAGE::$gt[$user_home[WEBPAGE::$userID]],WEBPAGE::$gt['fn']['TB.SCR.account']));
$tpl->setVariable('user_home_title',   WEBPAGE::$gt[$user_home[WEBPAGE::$userID]]);

$tpl->addBlockfile('user_home','user_home',sprintf("%s.tpl",$user_home[WEBPAGE::$userID]));
$tpl->setCurrentBlock("user_home");
if ( file_exists(sprintf("./includes/%s.inc",$user_home[WEBPAGE::$userID]) ))
{
  include sprintf("./includes/%s.inc",$user_home[WEBPAGE::$userID]);
}
$tpl->parseCurrentBlock("user_home") ;

