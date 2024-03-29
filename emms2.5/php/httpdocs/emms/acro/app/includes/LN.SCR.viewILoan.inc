<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'Date.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/roles.php';
require_once 'class/client.php';
require_once 'class/society.php';
require_once 'class/business_type.php';
require_once 'class/business.php';

$loan  = new LOAN($_REQUEST['id']); 
extract(LOAN::getTemplateData($_REQUEST['id']));

$btn  = new TTFButton(WEBPAGE::$theme);

if (count($status_controls))
{
  $c = 0;
  $separator = '';
  $tpl->setVariable('status_bar_label', WEBPAGE::$gt['statusSelector']);
  $tpl->addBlockfile('status_bar','status_bar','ST.navbar.tpl');
  foreach ($status_controls as $key => $val)
  {
    $tpl->setCurrentBlock("st_navbar");
    $tpl->setVariable('width',     sprintf("%spx",(30*$c)));
    $tpl->setVariable('separator', $separator);
    $tpl->setVariable('link',      $val['link']);
    $tpl->setVariable('text',      $val['text']);
    $tpl->parseCurrentBlock("st_navbar");
    $c = 1;
    $separator = '|';
  }
}

$tpl->setVariable('xpress_title',    WEBPAGE::$gt['LN.SCR.viewILoan']);
$tpl->setVariable('loan_code',       $loan_code);
$tpl->setVariable('loan_code_label', WEBPAGE::$gt['tblLoans.loan_code']);
$tpl->setVariable('status',          WEBPAGE::$gt[sprintf("tblLoans.status.%s",$status)]);
$tpl->setVariable('status_label',    WEBPAGE::$gt['tblLoans.status']);
$tpl->setVariable('borrower_type',   WEBPAGE::$gt[sprintf('tblLoanTypes.borrower_type.%s',$borrower_type)]);
$tpl->setVariable('borrower_name',   $borrower_name);
$tpl->setVariable('href_borrower',   $borrower_link);
$tpl->setVariable('borrower_code',   $borrower_code);
$tpl->setVariable('borrower_label',  WEBPAGE::$gt['tblClients.id']);
$tpl->setVariable('currency',        $currency);
$tpl->setVariable('currency_label',  WEBPAGE::$gt['tblFunds.currency_id']);
if (WEBPAGE::$conf['app']['business_check'])
{
  $tpl->setVariable('business_label', WEBPAGE::$gt['tblBusiness.name']);
  if ($business_id)
  {
    $business = new BUSINESS($business_id);
    $tpl->setVariable('business',      $business->data['name']);
    $tpl->setVariable('href_business', sprintf('index.php?scr_name=LN.SCR.pickBusiness&loan_id=%s&client_id=%s',$_REQUEST['id'],$client_id));
  } else {
    $tpl->setVariable('business',      WEBPAGE::$gt['select']);
    $tpl->setVariable('href_business', sprintf('index.php?scr_name=LN.SCR.pickBusiness&loan_id=%s&client_id=%s',$_REQUEST['id'],$client_id));
  }
}
$tpl->setVariable('survey',                     $survey);
$tpl->setVariable('href_survey',                $href_survey);
$tpl->setVariable('survey_label',               WEBPAGE::$gt['tblSurveys.id']);
$tpl->setVariable('modality',                   $modality);
$tpl->setVariable('modality_label',             WEBPAGE::$gt['tblLoanTypes.description']);
$tpl->setVariable('kp',                         $kp);
$tpl->setVariable('kp_label',                   WEBPAGE::$gt['tblLoans.kp']);
$tpl->setVariable('collateralType_label',       WEBPAGE::$gt['tblCollateral.id']);
$tpl->setVariable('currency_symbol',            $currency_symbol);
$tpl->setVariable('collateral_min_value',       $collateral_min_value);
$tpl->setVariable('collateral_min_value_label', WEBPAGE::$gt['tblLoanTypes.margin_c']);
$tpl->setVariable('pg_value',                   $pg_value);
$tpl->setVariable('pg_value_label',             WEBPAGE::$gt['tblCollateral.pawned_goods']);
$tpl->setVariable('re_value',                   $re_value);
$tpl->setVariable('re_value_label',             WEBPAGE::$gt['tblCollateral.real_estate']);
$tpl->setVariable('fgd_value',                  $fgd_value);
$tpl->setVariable('fgd_value_label',            WEBPAGE::$gt['tblCollateral.fiduciary_goods']);
$tpl->setVariable('fgt_value',                  $fgt_value);
$tpl->setVariable('fgt_value_label',            WEBPAGE::$gt['tblCollateral.fiduciary_garantees']);
$tpl->setVariable('collateral_value',           $collateral_value);
$tpl->setVariable('collateral_value_label',     WEBPAGE::$gt['total']);
$tpl->setVariable('calendar_type',              WEBPAGE::$gt[sprintf('tblLoanTypes.calendar_type.%s',$calendar_type)]);
$tpl->setVariable('calendar_type_label',        WEBPAGE::$gt['tblLoanTypes.calendar_type']);
$tpl->setVariable('days',                       WEBPAGE::$gt['days']);
$tpl->setVariable('margin_label',               WEBPAGE::$gt['margin']);
$tpl->setVariable('margin_k',                   $margin_k);
$tpl->setVariable('margin_k_label',             WEBPAGE::$gt['principal']);
$tpl->setVariable('margin_r',                   $margin_r);
$tpl->setVariable('margin_r_label',             WEBPAGE::$gt['interest']);
$tpl->setVariable('margin_d',                   $margin_d);
$tpl->setVariable('margin_d_label',             WEBPAGE::$gt['tblLoanTypes.margin_d']);
$tpl->setVariable('savings_p',                  $savings_p);
$tpl->setVariable('savings_p_label',            WEBPAGE::$gt['tblLoans.savings_p']);
$tpl->setVariable('savings_v',                  $savings_v);
$tpl->setVariable('savings_v_label',            WEBPAGE::$gt['tblLoans.savings_v']);
$tpl->setVariable('installment',                $installment);
$tpl->setVariable('installment_label',          WEBPAGE::$gt['tblLoanTypes.installment']);
$tpl->setVariable('payment_frequency',          WEBPAGE::$gt[sprintf('tblLoanTypes.payment_frequency.%s',$payment_frequency)]);
$tpl->setVariable('payment_frequency_label',    WEBPAGE::$gt['tblLoanTypes.payment_frequency']);
$tpl->setVariable('rates_label',                WEBPAGE::$gt['rate']);
$tpl->setVariable('fees_at_label',              WEBPAGE::$gt['tblLoanTypes.fees_at']);
$tpl->setVariable('fees_at',                    $fees_at);
$tpl->setVariable('fees_af_label',              WEBPAGE::$gt['tblLoanTypes.fees_af']);
$tpl->setVariable('fees_af',                    $fees_af);
$tpl->setVariable('rates_r_label',              WEBPAGE::$gt['tblLoanTypes.rates_r']);
$tpl->setVariable('rates_r',                    $rates_r);
$tpl->setVariable('rates_e_label',              WEBPAGE::$gt['rate_e']);
$tpl->setVariable('rates_e',                    $rates_e);
$tpl->setVariable('rates_d_label',              WEBPAGE::$gt['tblLoanTypes.rates_d']);
$tpl->setVariable('rates_d',                    $rates_d);
$tpl->setVariable('creator',                    sprintf(WEBPAGE::$gt['createdByOn'],$creator,$f_creator_date));
$tpl->setVariable('editor',                     sprintf(WEBPAGE::$gt['editedByOn'],$editor,$f_editor_date));
$tpl->setVariable('img',                        $borrower_img_path);

include './includes/ST.SCR.buttonMan.inc';

?>