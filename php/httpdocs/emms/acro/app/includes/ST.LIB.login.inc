<?php
function loginFunction() 
{
  global $message;
  if (!$message) $message = isset($_REQUEST['msg']) ? $_REQUEST['msg'] : 'pleaseLogin';
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
  WEBPAGE::$lang = isset($_POST['lang']) ? $_POST['lang'] : (isset($_GET['lang']) ? $_GET['lang']  :  WEBPAGE::$conf['app']['default_language']);
  WEBPAGE::load_gt();

  $cache = sprintf(WEBPAGE::_APP_QUERY_CACHE,WEBPAGE::$paths['3'],WEBPAGE::$paths['2'],str_pad($_SESSION['_authsession']['data']['id'],3,'0',STR_PAD_LEFT));
  foreach (glob($cache.".*") as $filename) { unlink($filename); }
  $menu = simplexml_load_file(sprintf(WEBPAGE::_APP_MENU,WEBPAGE::$paths['3'],WEBPAGE::$paths['2']));
  foreach($menu->children() as $mc)
  {
    $m[] = $mc;
  }
  $url = parse_url($_SERVER['HTTP_REFERER']);
  $_SESSION['_authsession']['data']['module']      = 'main';
  $_SESSION['_authsession']['data']['url']         = sprintf('%s://%s%s',$url['scheme'],$url['host'],$url['path']);
  $_SESSION['_authsession']['data']['theme']       = WEBPAGE::$theme;
  $_SESSION['_authsession']['data']['screenWidth'] = WEBPAGE::$screenWidth;
  $_SESSION['_authsession']['data']['lang']        = WEBPAGE::$lang;
  $_SESSION['_authsession']['data']['navtree']     = genNavTree($m[0]);
  $_SESSION['_authsession']['data']['tabmenu']     = genTabMenu($m[1]);
  $_SESSION['_authsession']['data']['zoneName']    = current(SQL::getAssoc('tblZones','id,zone',sprintf("id='%s'",$_SESSION['_authsession']['data']['zone_id'])));
  if ( date('Y-m-d') >= current(WEBPAGE::$dbh->getAssoc(sprintf('select id,force_next_pwd_date from tblUsers where id = "%s"',$_SESSION['_authsession']['data']['id']))))
  {
    $_SESSION['_authsession']['data']['access_code'] = 0;
    WEBPAGE::redirect('index.password.php');
  } else { 
    WEBPAGE::redirect('index.php');
  }
  exit;
}

function encryptPass() 
{
  $salt = md5($_POST['password']);
  $EncryptPass = crypt($_POST['password'], $salt);
  return $EncryptPass;
}

function genTabMenu($m)
{ 
  $i = 0;
  foreach ($m->children() as $mc)
  {
    $tabmenu[$i]['URL'] = $mc['popup'] ? sprintf('javascript:openWin("index.popup.php?scr_name=%s","%s","menubar=no,scrollbars=yes,resizable=yes,width=%s,height=%s")',$mc['name'],$mc['name'],$mc['width'],$mc['height']) : sprintf('index.php?scr_name=%s',$mc['name']);
    $tabmenu[$i]['TAB'] = WEBPAGE::$gt['fn'][sprintf($mc['name'])];
    $i++;
  }
  return $tabmenu;
}

function genNavTree($m)
{
  require_once('./class/TreeMenu.php'); 
  $n = parseXTree(new HTML_TreeNode(array('text'=>WEBPAGE::$gt['menu'],'cssClass'=>WEBPAGE::_TREEMENU_CSS_CAPTION)),$m);
  $menu = new HTML_TreeMenu();
  $menu->addItem($n);
  $treeMenu = &new HTML_TreeMenu_DHTML($menu, array('images' => sprintf(WEBPAGE::_TREEMENU_IMG_PATH,WEBPAGE::$conf['app']['default_theme']))); 
  $navtree = $treeMenu->toHTML();
  return $navtree;
}

function parseXTree(&$node,$mp)
{
  foreach($mp->children()->node as $mc)
  {
    if ( intval($_SESSION['_authsession']['data']['access_code']) & intval($mc['code']) )
    {
      $options = array
                 (
                   'text' => WEBPAGE::$gt['fn'][sprintf($mc['name'])],
                   'link' => $mc->node['name'] ? '' : ($mc['popup'] ? sprintf("javascript:openWin(\'index.popup.php?scr_name=%s\',\'%s\',\'menubar=no,scrollbars=yes,resizable=yes,width=%s,height=%s\')",$mc['name'],$mc['name'],$mc['width'],$mc['height']) : 'index.php?scr_name='.$mc['name']),
                   'icon' => $mc->node['name'] ? WEBPAGE::_TREEMENU_ICON_FOLDER : WEBPAGE::_TREEMENU_ICON_FILE,
                   'expandedIcon' => WEBPAGE::_TREEMENU_ICON_XFOLDER,
                   'cssClass' => $mc->node['name'] ? WEBPAGE::_TREEMENU_CSS_BRANCH : WEBPAGE::_TREEMENU_CSS_NODE
                 );
      $n = $node->addItem(new HTML_TreeNode($options));
      parseXTree($n,$mc);
    }
  } 
  return $node;
}

?>