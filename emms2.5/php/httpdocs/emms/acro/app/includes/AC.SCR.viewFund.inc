<?php
require_once 'class/fund.php';

extract(FUND::getTemplateData($_REQUEST['id']));

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('fund',				$fund);
$tpl->setVariable('code_label',			WEBPAGE::$gt['tblFunds.code']);
$tpl->setVariable('code',				$code);
$tpl->setVariable('currency_label',		WEBPAGE::$gt['tblFunds.currency_id']);
$tpl->setVariable('currency',			$currency);
$tpl->setVariable('status_label',		WEBPAGE::$gt['tblFunds.status']);
$tpl->setVariable('status',				WEBPAGE::$gt[sprintf('tblFunds.status.%s',$status)]);
$tpl->setVariable('description_label',	WEBPAGE::$gt['tblFunds.description']);
$tpl->setVariable('description',		$description);
$tpl->setVariable('creator',			sprintf(WEBPAGE::$gt['createdByOn'],$creator,$f_creator_date));
$tpl->setVariable('editor',				sprintf(WEBPAGE::$gt['editedByOn'],$editor,$f_editor_date));
$tpl->setVariable('fund_img',			$img_path);

include './includes/ST.SCR.buttonMan.inc';

?>