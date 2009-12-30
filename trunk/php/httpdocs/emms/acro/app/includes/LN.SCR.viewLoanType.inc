<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_type.php';

extract(LOAN_TYPE::getTemplateData($_REQUEST['id']));

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('description',             $description);
$tpl->setVariable('borrower_type_label',     WEBPAGE::$gt['tblLoanTypes.borrower_type']);
$tpl->setVariable('borrower_type',           WEBPAGE::$gt[sprintf('tblLoanTypes.borrower_type.%s',$borrower_type)]);
$tpl->setVariable('calendar_label',          WEBPAGE::$gt['tblLoanTypes.calendar']);
$tpl->setVariable('calendar',                WEBPAGE::$gt[sprintf('tblLoanTypes.calendar.%s',$calendar)]);
$tpl->setVariable('currency_label',          WEBPAGE::$gt['tblFunds.currency_id']);
$tpl->setVariable('currency',                $currency);
$tpl->setVariable('installment_label',       WEBPAGE::$gt['tblLoanTypes.installment']);
$tpl->setVariable('installment',             $installment);
$tpl->setVariable('payment_frequency_label', WEBPAGE::$gt['tblLoanTypes.payment_frequency']);
$tpl->setVariable('payment_frequency',       WEBPAGE::$gt[sprintf('tblLoanTypes.payment_frequency.%s',$payment_frequency)]);
$tpl->setVariable('margin_c_label',          WEBPAGE::$gt['tblLoanTypes.margin_c']);
$tpl->setVariable('margin_c',                $margin_c);
$tpl->setVariable('margin_d_label',          WEBPAGE::$gt['tblLoanTypes.margin_d']);
$tpl->setVariable('margin_d',                $margin_d);
$tpl->setVariable('savings_p_label',         WEBPAGE::$gt['tblLoanTypes.savings_p']);
$tpl->setVariable('savings_p',               $savings_p);
$tpl->setVariable('fees_at_label',           WEBPAGE::$gt['tblLoanTypes.fees_at']);
$tpl->setVariable('fees_at',                 $fees_at);
$tpl->setVariable('fees_af_label',           WEBPAGE::$gt['tblLoanTypes.fees_af']);
$tpl->setVariable('fees_af',                 $fees_af);
$tpl->setVariable('rates_r_label',           WEBPAGE::$gt['tblLoanTypes.rates_r']);
$tpl->setVariable('rates_r',                 $rates_r);
$tpl->setVariable('rates_e_label',           WEBPAGE::$gt['rate_e']);
$tpl->setVariable('rates_e',                 $rates_e);
$tpl->setVariable('rates_d_label',           WEBPAGE::$gt['tblLoanTypes.rates_d']);
$tpl->setVariable('rates_d',                 $rates_d);
$tpl->setVariable('creator',                 sprintf(WEBPAGE::$gt['createdByOn'],$creator,$f_creator_date));
$tpl->setVariable('editor',                  sprintf(WEBPAGE::$gt['editedByOn'],$editor,$f_editor_date));
$tpl->setVariable('ltype_img',               $img_path);

$tpl->setVariable('installment_lock', $installment_lock ? '' : WEBPAGE::$gt['FLEX']);
$tpl->setVariable('rates_r_lock',     $rates_r_lock     ? '' : WEBPAGE::$gt['FLEX']);
$tpl->setVariable('rates_d_lock',     $rates_d_lock     ? '' : WEBPAGE::$gt['FLEX']);
$tpl->setVariable('fees_at_lock',     $fees_at_lock     ? '' : WEBPAGE::$gt['FLEX']);
$tpl->setVariable('fees_af_lock',     $fees_af_lock     ? '' : WEBPAGE::$gt['FLEX']);
$tpl->setVariable('margin_d_lock',    $margin_d_lock    ? '' : WEBPAGE::$gt['FLEX']);
$tpl->setVariable('margin_c_lock',    $margin_c_lock    ? '' : WEBPAGE::$gt['FLEX']);
$tpl->setVariable('savings_p_lock',   $savings_p_lock   ? '' : WEBPAGE::$gt['FLEX']);

include './includes/ST.SCR.buttonMan.inc';

?>


