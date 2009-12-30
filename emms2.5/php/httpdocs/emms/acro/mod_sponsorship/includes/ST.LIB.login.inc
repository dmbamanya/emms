<?
function loginFunction()
{
  global $message;
  if (!$message) { $message = 'pleaseLogin'; }
  require './includes/login.inc';
  exit;
}

function loginFailedCallback()
{
  global $message;
  $message = 'invalidLogin';
  loginFunction();
}

function loginCallback()
{
  /*
   * set app language & load localized texts
   */
  WEBPAGE::$lang = isset($_POST['lang']) ? $_POST['lang'] : (isset($_GET['lang']) ? $_GET['lang']  :  WEBPAGE::$conf['mod_sponsorship']['default_language']);
  WEBPAGE::load_gt();

  $cache = sprintf(WEBPAGE::_MOD_SPONSORSHIP_QUERY_CACHE,WEBPAGE::$paths['3'],WEBPAGE::$paths['2'],str_pad($_SESSION['_authsession']['data']['id'],6,'0',STR_PAD_LEFT));
  foreach (glob($cache.".*") as $filename) { unlink($filename); }

  $url = parse_url($_SERVER['HTTP_REFERER']);
  $_SESSION['_authsession']['data']['module']      = 'mod_sponsorship';
  $_SESSION['_authsession']['data']['url']         = sprintf('%s://%s%s',$url['scheme'],$url['host'],$url['path']);
  $_SESSION['_authsession']['data']['lang']        = WEBPAGE::$lang;
  $_SESSION['_authsession']['data']['partner']     = WEBPAGE::$partner;
  $_SESSION['_authsession']['data']['theme']       = WEBPAGE::$theme;
  $_SESSION['_authsession']['data']['screenWidth'] = WEBPAGE::$screenWidth;
  WEBPAGE::$dbh->query(sprintf("insert into tblSponsorsLog (sponsor_id,ip_address,login_date,last_hit_date) values ('%s','%s',NOW(),NOW())",$_SESSION['_authsession']['data']['id'],$_SERVER['REMOTE_ADDR']));
  $_SESSION['_authsession']['data']['session_id']  = mysql_insert_id(WEBPAGE::$dbh->connection);
  WEBPAGE::redirect('index.php');
  exit;
}

function encryptPass()
{
  $salt = md5($_POST['password']);
  $EncryptPass = crypt($_POST['password'], $salt);
  return $EncryptPass;
}


?>