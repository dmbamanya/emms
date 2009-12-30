<?php 
require_once '../app/class/emms.php';

class WEBPAGE extends EMMS
{
  static $partner;

  function WEBPAGE()
  {
    self::$auth_options = array('dsn' => '','table' => 'tblSponsors','usernamecol' => 'username','passwordcol' => 'password','cryptType' => 'encryptPass','db_fields' => '*');
    self::EMMS();
    self::$url = self::$conf['mod_sponsorship']['url'];
    self::$partner = $_REQUEST['partner'] ? $_REQUEST['partner'] : '';
  }

  static function START()
  {
    $start = new WEBPAGE();
  }

  static function LOAD_SESSION()
  {
    self::$lang			= &$_SESSION['_authsession']['data']['lang'];
    self::$partner		= &$_SESSION['_authsession']['data']['partner'];
    self::$screenWidth 		= &$_SESSION['_authsession']['data']['screenWidth'];
    self::$userID 		= &$_SESSION['_authsession']['data']['id'];
    self::$userAccessCode	= &$_SESSION['_authsession']['data']['access_code'];
    self::$userName		= &$_SESSION['_authsession']['data']['first'];
    self::$queryCache		= sprintf(self::_MOD_SPONSORSHIP_QUERY_CACHE,self::$paths['3'],self::$paths['2'],str_pad(self::$userID,6,'0',STR_PAD_LEFT));
  }

  static function load_fn()
  {
      
  }

  static function redirect($url)
  {
    $location = sprintf("Location: %s%s",self::$conf['mod_sponsorship']['url'],$url);
    header($location);
    exit;
  }
  
}
?>
