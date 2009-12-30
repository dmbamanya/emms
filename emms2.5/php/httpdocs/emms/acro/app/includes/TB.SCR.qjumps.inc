<?php
require_once 'class/uprofile.php';
$roles = UPROFILE::getRoles($_SESSION['_authsession']['data']['access_code']);

$qjumps = WEBPAGE::$dbh->getAssoc('select qjump,qjump from tblQjumps where user_id = '. WEBPAGE::$userID);

$i = 1;
foreach($qjumps as $qj)
{
  if(WEBPAGE::validateFn($qj))
  {
    $tpl->setVariable(sprintf('qjump_%s',$i), WEBPAGE::$gt['fn'][$qj]);
    $tpl->setVariable(sprintf('qjump_%s_img',$i), sprintf('%s/icons/48x48/%s.png',WEBPAGE::$themePath,$qj));
    $tpl->setVariable(sprintf('qjump_%s_url',$i), WEBPAGE::$fn['url'][$qj]);
    $i++;
  }
}
$tpl->setVariable('roles_caption', WEBPAGE::$gt['tblUsers.access_code']);
$tpl->setVariable('roles',         implode(', ',$roles));
$tpl->setVariable('msg',           sprintf(WEBPAGE::$gt['quick_jumps_tip'],WEBPAGE::$gt['fn']['TB.SCR.account']));
?>
