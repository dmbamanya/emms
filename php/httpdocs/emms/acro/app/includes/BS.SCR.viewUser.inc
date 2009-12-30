<?php
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/user.php';

extract(USER::getTemplateData($_REQUEST['id']));
array_walk($qjump,'x_qjump');

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('user_name',         $name);
$tpl->setVariable('username_label',    WEBPAGE::$gt['tblUsers.username']);
$tpl->setVariable('username',          $username);
$tpl->setVariable('code_label',        WEBPAGE::$gt['tblUsers.code']);
$tpl->setVariable('code',              $code);
$tpl->setVariable('email_label',       WEBPAGE::$gt['tblUsers.email']);
$tpl->setVariable('email',             $email);
$tpl->setVariable('super_label',       WEBPAGE::$gt['tblUsers.super_id']);
$tpl->setVariable('super',             $super);
$tpl->setVariable('zone_label',        WEBPAGE::$gt['tblZones.zone']);
$tpl->setVariable('zone',              $zone);
$tpl->setVariable('birthdate_label',   WEBPAGE::$gt['tblUsers.birthdate']);
$tpl->setVariable('birthdate',         $f_birthdate);
$tpl->setVariable('gender_label',      WEBPAGE::$gt['tblUsers.gender']);
$tpl->setVariable('gender',            WEBPAGE::$gt[sprintf('tblUsers.gender.%s',$gender)]);
$tpl->setVariable('access_code_label', WEBPAGE::$gt['tblUsers.access_code']);
$tpl->setVariable('access_code',       implode('<br>',$membership));
$tpl->setVariable('memo_label',        WEBPAGE::$gt['tblUsers.memo']);
$tpl->setVariable('memo',              $memo);
$tpl->setVariable('creator_label',     WEBPAGE::$gt['tblUsers.creator_id']);
$tpl->setVariable('creator',           sprintf(WEBPAGE::$gt['createdByOn'],$creator,$f_creator_date));
$tpl->setVariable('editor_label',      WEBPAGE::$gt['tblUsers.editor_id']);
$tpl->setVariable('editor',            sprintf(WEBPAGE::$gt['editedByOn'],$editor,$f_editor_date));
$tpl->setVariable('staff_label',       WEBPAGE::$gt['tblUsers.staff']);
$tpl->setVariable('staff',             WEBPAGE::$gt[sprintf('tblUsers.staff.%s',$staff)]);
$tpl->setVariable('home_label',        WEBPAGE::$gt['tblUsers.home']);
$tpl->setVariable('home',              WEBPAGE::$gt['fn'][$home]);
$tpl->setVariable('qjump_label',       WEBPAGE::$gt['tblQjumps.qjump']);
$tpl->setVariable('qjump',             implode('<br />',$qjump));
$tpl->setVariable('user_img',          $img_path);

include './includes/ST.SCR.buttonMan.inc';

function x_qjump(&$val,$key)
{
  $val = WEBPAGE::$gt['fn'][$key];
}

?>