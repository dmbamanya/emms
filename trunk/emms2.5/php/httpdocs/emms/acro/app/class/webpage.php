<?php 
require_once 'class/emms.php';

class WEBPAGE extends EMMS
{
  function WEBPAGE()
  {
    self::$auth_options = array('dsn' => '','table' => 'tblUsers','usernamecol' => 'username','passwordcol' => 'password','cryptType' => 'encryptPass','db_fields' => '*');
    self::EMMS();
    self::$url = self::$conf['app']['url'];
  }

  static function START()
  {
    $start = new WEBPAGE();
  }

  static function LOAD_SESSION()
  {
    self::$lang			= &$_SESSION['_authsession']['data']['lang'];
    self::$screenWidth 		= &$_SESSION['_authsession']['data']['screenWidth'];
    self::$userID 		= &$_SESSION['_authsession']['data']['id'];
    self::$userAccessCode	= &$_SESSION['_authsession']['data']['access_code'];
    self::$userName		= &$_SESSION['_authsession']['data']['first'];
    self::$userZone		= &$_SESSION['_authsession']['data']['zone_id'];
    self::$zoneName		= &$_SESSION['_authsession']['data']['zoneName'];
    self::$tabmenu		= &$_SESSION['_authsession']['data']['tabmenu'];
    self::$navtree		= &$_SESSION['_authsession']['data']['navtree'];
    self::$queryCache		= sprintf(self::_APP_QUERY_CACHE,self::$paths['3'],self::$paths['2'],str_pad(self::$userID,3,'0',STR_PAD_LEFT));
  }

  static function load_fn()
  {
    if (!(file_exists($cache = sprintf(self::_APP_FN_CACHE,self::$paths['3'],self::$paths['2'],self::$lang))))
    {
      /*
       * gen all fn masks, paths and parents from menu.xml and write to cache
       */
      $menu = simplexml_load_file(sprintf(self::_APP_MENU,self::$paths['3'],self::$paths['2']));
      self::gen_fnData($menu);
      self::makecachefile(self::$fn,$cache);
    } else {
    /*
     * load fn paths from cache
     */
      self::$fn = self::getCacheData($cache);
    }
  }

  static function load_fnData($menu,$fn = '')
  {
    foreach($menu->children() as $mc)
    {
      self::$fn['parent'][sprintf($mc['name'])] = $fn;
      self::$fn['mask'][sprintf($mc['name'])] = sprintf($mc['code']);
      if ($mc['qjump']) { self::$fn['qjump'][sprintf($mc['name'])] = sprintf($mc['qjump']); }
      if ($mc['popup']) { self::$fn['popup'][sprintf($mc['name'])] = array('width'=>$mc['width'],'height'=>$mc['height']); }
      self::$fn['url'][sprintf($mc['name'])] = $mc['popup'] ? sprintf("javascript:openWin('index.popup.php?scr_name=%s','%s','menubar=no,scrollbars=yes,resizable=%s,width=%s,height=%s')",$mc['name'],$mc['name'],$mc['resizable'] ? $mc['resizable'] : 'yes',$mc['width'],$mc['height']) : 'index.php?scr_name='.$mc['name'];
      self::load_fnData($mc,sprintf($mc['name']));
    }
  }

  static function gen_fnData($menu)
  {
    self::load_fnData($menu);
    foreach (self::$fn['parent'] as $fn => $parent)
    {
      $ch = $fn;
      self::$fn['path'][$fn] = self::$gt['fn'][$fn];
      while (self::$fn['parent'][$ch] != '')
      {
        self::$fn['path'][$fn] = sprintf('%s > %s', self::$gt['fn'][self::$fn['parent'][$ch]],self::$fn['path'][$fn]);
        $ch = self::$fn['parent'][$ch];
      }
    }
  }

  static function redirect($url)
  {
    $location = sprintf("Location: %s%s",self::$conf['app']['url'],$url);
    header($location);
    exit;
  }
  
}
?>
