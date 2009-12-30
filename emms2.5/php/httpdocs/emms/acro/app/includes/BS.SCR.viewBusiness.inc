<?php
require_once 'class/client.php';
require_once 'class/business_type.php';
require_once 'class/business.php';

extract(BUSINESS::getTemplateData($_REQUEST['id']));

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('name',				$name);
$tpl->setVariable('name_label',			WEBPAGE::$gt['tblBusiness.name']);
$tpl->setVariable('activity',			WEBPAGE::$gt[sprintf('tblBusinessTypes.activity.%s',$activity)]);
$tpl->setVariable('activity_label',		WEBPAGE::$gt['tblBusinessTypes.activity']);
$tpl->setVariable('type',				$type);
$tpl->setVariable('type_label',			WEBPAGE::$gt['tblBusiness.type_id']);
$tpl->setVariable('owners',				implode('<br>',WEBPAGE::hotlist('BS.SCR.viewClient','id',$owners)));
$tpl->setVariable('owners_label',		WEBPAGE::$gt['tblBusiness.client_list']);
$tpl->setVariable('description',		$description);
$tpl->setVariable('description_label',	WEBPAGE::$gt['tblBusiness.description']);
$tpl->setVariable('creator',			sprintf(WEBPAGE::$gt['createdByOn'],$creator,$f_creator_date));
$tpl->setVariable('editor',				sprintf(WEBPAGE::$gt['editedByOn'],$editor,$f_editor_date));
$tpl->setVariable('bz_img',				$img_path);

include './includes/ST.SCR.buttonMan.inc';


?>