<?php
require_once 'class/zone.php';

extract(ZONE::getTemplateData($_REQUEST['id']));

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('zone',			$zone);
$tpl->setVariable('short_name',		$short_name);
$tpl->setVariable('zA_label',		WEBPAGE::$gt['tblZones.zA']);
$tpl->setVariable('zA',				$zA);
$tpl->setVariable('zB_label',		WEBPAGE::$gt['tblZones.zB']);
$tpl->setVariable('zB',				$zB);
$tpl->setVariable('zC_label',		WEBPAGE::$gt['tblZones.zC']);
$tpl->setVariable('zC',				$zC);
$tpl->setVariable('programs_label',	WEBPAGE::$gt['tblPrograms.id']);
$tpl->setVariable('programs',		implode('<br>',WEBPAGE::hotlist('BS.SCR.viewProgram','id',ZONE::getPrograms($_REQUEST['id']))));
$tpl->setVariable('memo_label',		WEBPAGE::$gt['tblZones.memo']);
$tpl->setVariable('memo',			$memo);
$tpl->setVariable('creator',		sprintf(WEBPAGE::$gt['createdByOn'],$creator,$f_creator_date));
$tpl->setVariable('editor',			sprintf(WEBPAGE::$gt['editedByOn'],$editor,$f_editor_date));

include './includes/ST.SCR.buttonMan.inc';

?>