<?php
require_once '../mod_sponsorship/class/sponsor.php';
$btn  = new TTFButton(WEBPAGE::$theme);

$sponsor = new SPONSOR($_REQUEST['id']);
$sponsor->loadExtendedInfo();

$tpl->setVariable('sponsor',				     $sponsor->data['sponsor']);
$tpl->setVariable('username',				     $sponsor->data['username']);
$tpl->setVariable('username_label',			 WEBPAGE::$gt['tblUsers.username']);
$tpl->setVariable('contact_name',				 sprintf("%s %s %s",$sponsor->data['first'],$sponsor->data['middle'],$sponsor->data['last']));
$tpl->setVariable('contact_name_label',  WEBPAGE::$gt['name']);
$tpl->setVariable('contact_email',		   $sponsor->data['email']);
$tpl->setVariable('contact_email_label', WEBPAGE::$gt['tblUsers.email']);
$tpl->setVariable('status',				       WEBPAGE::$gt[sprintf("tblUsers.active.%s",$sponsor->data['active'])]);
$tpl->setVariable('status_label',		     WEBPAGE::$gt['tblUsers.active']);

$allzones = $sponsor->getZones();
$programs = array();
foreach($sponsor->data['zone_id'] as $zone_id) {
  $programs[] = $allzones[$zone_id];
  }

$tpl->setVariable('program',                 implode('<br />',$programs));
$tpl->setVariable('program_label',           WEBPAGE::$gt['tblPrograms.program']);
$tpl->setVariable('memo',		             $sponsor->data['memo']);
$tpl->setVariable('memo_label',	         WEBPAGE::$gt['memo']);
$tpl->setVariable('creator',			       sprintf(WEBPAGE::$gt['createdByOn'],$sponsor->data['creator'],$sponsor->data['f_creator_date']));
$tpl->setVariable('editor',				       sprintf(WEBPAGE::$gt['editedByOn'],$sponsor->data['editor'],$sponsor->data['f_editor_date']));

$buttondata = $sponsor->data['buttondata'];
include './includes/ST.SCR.buttonMan.inc';

?>