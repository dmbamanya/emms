<?
/*
 * Set cookie's secure flag if using ssl
 */
$_SERVER['HTTPS'] ? session_set_cookie_params(0,'/','',true,true) :  session_set_cookie_params(0,'/','',false,true);

require_once './class/TTFButton.php';
$btn = new TTFButton( isset($_GET['theme']) ?  $_GET['theme'] : '', $_GET['txt'] ) ;
$btn->show();
?>