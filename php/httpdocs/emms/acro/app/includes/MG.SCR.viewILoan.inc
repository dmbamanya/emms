<?php
exit;
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

$tpl->setVariable('loan_code',					$loan_code);
$tpl->setVariable('loan_code_label',			WEBPAGE::$gt['tblLoans.loan_code']);
$tpl->setVariable('status',						WEBPAGE::$gt[sprintf("tblLoans.status.%s",$status)]);
$tpl->setVariable('status_label',				WEBPAGE::$gt['tblLoans.status']);
$tpl->setVariable('borrower_type',				WEBPAGE::$gt[sprintf('tblLoanTypes.borrower_type.%s',$borrower_type)]);
$tpl->setVariable('borrower_name',				$borrower_name);
$tpl->setVariable('href_borrower',				$borrower_link);
$tpl->setVariable('borrower_code',				$borrower_code);
$tpl->setVariable('borrower_label',				WEBPAGE::$gt['tblClients.id']);
$tpl->setVariable('currency',					$currency);
$tpl->setVariable('currency_label',				WEBPAGE::$gt['tblFunds.currency_id']);
if (WEBPAGE::$conf['app']['business_check']) {
  $tpl->setVariable('business_label', 			WEBPAGE::$gt['tblBusiness.name']);  
  if ($business_id) {
    $business	= new BUSINESS($business_id);
    $tpl->setVariable('business', 				$business->data['name']);
    $tpl->setVariable('href_business', 			sprintf('index.php?scr_name=LN.SCR.pickBusiness&loan_id=%s&client_id=%s',$_REQUEST['id'],$client_id));
    } else {
    $tpl->setVariable('business', 				WEBPAGE::$gt['select']);
    $tpl->setVariable('href_business', 			sprintf('index.php?scr_name=LN.SCR.pickBusiness&loan_id=%s&client_id=%s',$_REQUEST['id'],$client_id));
    }
  }
$tpl->setVariable('survey', 					$survey);
$tpl->setVariable('href_survey', 				$href_survey);
$tpl->setVariable('survey_label', 				WEBPAGE::$gt['tblSurveys.id']);
$tpl->setVariable('modality',					$modality);
$tpl->setVariable('modality_label',				WEBPAGE::$gt['tblLoanTypes.description']);
$tpl->setVariable('kp',							$kp);
$tpl->setVariable('kp_label',					WEBPAGE::$gt['tblLoans.kp']);
$tpl->setVariable('currency_symbol',			$currency_symbol);
$tpl->setVariable('savings_p', 					$savings_p);
$tpl->setVariable('savings_p_label', 			WEBPAGE::$gt['tblLoans.savings_p']);
$tpl->setVariable('savings_v', 					$savings_v);
$tpl->setVariable('savings_v_label', 			WEBPAGE::$gt['tblLoans.savings_v']);
$tpl->setVariable('installment',				$installment);
$tpl->setVariable('installment_label',			WEBPAGE::$gt['tblLoanTypes.installment']);
$tpl->setVariable('payment_frequency',			WEBPAGE::$gt[sprintf('tblLoanTypes.payment_frequency.%s',$payment_frequency)]);
$tpl->setVariable('payment_frequency_label',	WEBPAGE::$gt['tblLoanTypes.payment_frequency']);
$tpl->setVariable('rates_label', 				WEBPAGE::$gt['rate']);
$tpl->setVariable('fees_at_label',				WEBPAGE::$gt['tblLoanTypes.fees_at']);
$tpl->setVariable('fees_at',					$fees_at);
$tpl->setVariable('fees_af_label',				WEBPAGE::$gt['tblLoanTypes.fees_af']);
$tpl->setVariable('fees_af',					$fees_af);
$tpl->setVariable('rates_r_label',				WEBPAGE::$gt['tblLoanTypes.rates_r']);
$tpl->setVariable('rates_r',					$rates_r);
$tpl->setVariable('rates_e_label',				WEBPAGE::$gt['rate_e']);
$tpl->setVariable('rates_e',					$rates_e);
$tpl->setVariable('rates_d_label',				WEBPAGE::$gt['tblLoanTypes.rates_d']);
$tpl->setVariable('rates_d',					$rates_d);

$tpl->setVariable('pmt_label',					'Cuota');
$tpl->setVariable('pmt',						$pmt);

$tpl->setVariable('title',						'Datos del pr�stamo');
$tpl->setVariable('options',					'Opciones');

$tpl->setVariable('pmtPlan_href',				'javascript:openWin("index.popup.php?scr_name=LN.SCR.pmtPlan&ref=LN.SCR.viewILoan&id='.$id.'","pmtPlan","menubar=no,scrollbars=yes,resizable=yes,width=700,height=320")');
$tpl->setVariable('pmtPlan_label',				'Ver plan de pagos');  
$tpl->setVariable('remove_href',				sprintf("index.popup.php?scr_name=MG.SCR.removeILoan&id=%s&advisor_id=%s",$loan_id,$advisor_id));  
$tpl->setVariable('remove_label',				'Eliminar este record');  
$tpl->setVariable('new_href',					sprintf('index.popup.php?scr_name=MG.SCR.browseClients&advisor_id=%s',$advisor_id));  
$tpl->setVariable('new_label',					'Volver al listado de asociados');  


?>