<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_type.php';

$ltype = new LOAN_TYPE($_REQUEST['id']);
$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('ltypeForm','POST');

$form->addElement('static', 	      'submit', 	          '', $btn->imgsubmit('ltypeForm',WEBPAGE::$gt['submit']));
$form->addElement('static', 	      'reset', 	            '', $btn->imgreset('ltypeForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 	      'cancel', 	          '', $btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));

$form->addElement('hidden',		      'scr_name',		        $_REQUEST['scr_name']);
$form->addElement('hidden',		      'id',			            $_REQUEST['id']);
$form->addElement('hidden',		      'editor_id',			    WEBPAGE::$userID);
$form->addElement('hidden',		      'editor_date',			  date('Y-m-d'));

$form->addElement('static', 	      'rate', 	            '', WEBPAGE::$gt['rate']);
$form->addElement('static', 	      'margin', 	          '', WEBPAGE::$gt['margin']);
$form->addElement('static', 	      'insurance',          '', WEBPAGE::$gt['insurance']);

$form->addElement('select',		      'currency_id',			  WEBPAGE::$gt['tblFunds.currency_id'],			       (array('-'=>'') + $ltype->currencys()));
$form->addElement('select',		      'payment_frequency',	WEBPAGE::$gt['tblLoanTypes.payment_frequency'],	 (array('-'=>'') + $ltype->payment_frequencys()), 	"onChange='calEffRate()'");
$form->addElement('select',		      'borrower_type',		  WEBPAGE::$gt['tblLoanTypes.borrower_type'],		   (array('-'=>'') + $ltype->borrower_types()));
$form->addElement('select',		      'calendar_type',		  WEBPAGE::$gt['tblLoanTypes.calendar_type'],		   (array('-'=>'') + $ltype->calendar_types()), 		"onChange='calEffRate()'");
$form->addElement('text',		        'rates_r',				    WEBPAGE::$gt['tblLoanTypes.rates_r'],		         "onKeyUp='calEffRate()'");
$form->addElement('advcheckbox',    'rates_r_lock',		    '',		                                       WEBPAGE::$gt['lock'],'class=checkbox');
$form->addElement('text',		        'rates_d',				    WEBPAGE::$gt['tblLoanTypes.rates_d']);
$form->addElement('advcheckbox',    'rates_d_lock',		    '',		                                       WEBPAGE::$gt['lock'],'class=checkbox');
$form->addElement('text',		        'rates_e',				    WEBPAGE::$gt['rate_e'], 'disabled');
$form->addElement('text',		        'fees_at',				    WEBPAGE::$gt['tblLoanTypes.fees_at'],		         "onKeyUp='calEffRate()'");
$form->addElement('advcheckbox',    'fees_at_lock',		    '',		                                       WEBPAGE::$gt['lock'],'class=checkbox');
$form->addElement('text',		        'fees_af',				    WEBPAGE::$gt['tblLoanTypes.fees_af'],		         "onKeyUp='calEffRate()'");
$form->addElement('advcheckbox',    'fees_af_lock',		    '',		                                       WEBPAGE::$gt['lock'],'class=checkbox');
$form->addElement('text',		        'installment',			  WEBPAGE::$gt['tblLoanTypes.installment'],	       "onKeyUp='calEffRate()'");
$form->addElement('advcheckbox',    'installment_lock',	  '',		                                       WEBPAGE::$gt['lock'],'class=checkbox');
$form->addElement('text',		        'savings_p',			    WEBPAGE::$gt['tblLoanTypes.savings_p']);
$form->addElement('advcheckbox',    'savings_p_lock',		  '',		                                       WEBPAGE::$gt['lock'],'class=checkbox');
$form->addElement('text',		        'margin_d',				    WEBPAGE::$gt['tblLoanTypes.margin_d']);
$form->addElement('advcheckbox',    'margin_d_lock',		  '',		                                       WEBPAGE::$gt['lock'],'class=checkbox');
$form->addElement('text',		        'margin_c',				    WEBPAGE::$gt['tblLoanTypes.margin_c']);
$form->addElement('advcheckbox',    'margin_c_lock',		  '',		                                       WEBPAGE::$gt['lock'],'class=checkbox');
$form->addElement('text',		        'description',			  WEBPAGE::$gt['tblLoanTypes.description']);

//add rules
$form->addRule('currency_id',		    WEBPAGE::$gt['selectOption'],'required');
$form->addRule('currency_id',		    WEBPAGE::$gt['selectOption'],'alphanumeric');
$form->addRule('payment_frequency',	WEBPAGE::$gt['selectOption'],'required');
$form->addRule('payment_frequency',	WEBPAGE::$gt['selectOption'],'alphanumeric');
$form->addRule('borrower_type',		  WEBPAGE::$gt['selectOption'],'required');
$form->addRule('borrower_type',		  WEBPAGE::$gt['selectOption'],'alphanumeric');
$form->addRule('calendar_type',		  WEBPAGE::$gt['selectOption'],'required');
$form->addRule('calendar_type',		  WEBPAGE::$gt['selectOption'],'alphanumeric');

$form->addRule('description',		    sprintf(WEBPAGE::$gt['rangelength'],1,64),'required');
$form->addRule('description',		    sprintf(WEBPAGE::$gt['rangelength'],1,64),'rangelength', array(1,64));
$form->addRule('description',		    WEBPAGE::$gt['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');

$form->addRule('installment',	      WEBPAGE::$gt['numeric'],	'required');
$form->addRule('installment',	      WEBPAGE::$gt['numeric'],	'regex', '(^[0-9 ]*$)');
$form->addRule('rates_r',		        WEBPAGE::$gt['numeric'],	'required');
$form->addRule('rates_r',		        WEBPAGE::$gt['numeric'],	'numeric');
$form->addRule('rates_d',		        WEBPAGE::$gt['numeric'],	'required');
$form->addRule('rates_d',		        WEBPAGE::$gt['numeric'],	'numeric');
$form->addRule('fees_at',		        WEBPAGE::$gt['numeric'],	'required');
$form->addRule('fees_at',		        WEBPAGE::$gt['numeric'],	'numeric');
$form->addRule('fees_af',		        WEBPAGE::$gt['numeric'],	'required');
$form->addRule('fees_af',		        WEBPAGE::$gt['numeric'],	'numeric');
$form->addRule('savings_p',		      WEBPAGE::$gt['numeric'],	'required');
$form->addRule('savings_p',		      WEBPAGE::$gt['numeric'],	'numeric');
$form->addRule('margin_d',		      WEBPAGE::$gt['numeric'],	'required');
$form->addRule('margin_d',		      WEBPAGE::$gt['numeric'],	'regex', '(^[0-9 ]*$)');
$form->addRule('margin_c',		      WEBPAGE::$gt['numeric'],	'required');
$form->addRule('margin_c',		      WEBPAGE::$gt['numeric'],	'numeric');

$form->freeze($ltype->frostdata);

if ($form->validate()) { 

  $form->freeze();
  $values = $form->_submitValues;
  foreach ($values as $key=>$val) { $ltype->data[$key] = $val; }
  $ltype->data['id'] = $ltype->data['id'] ? $ltype->update() : $ltype->add();
  WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.viewLoanType&id=%s&ico=scs&msg=scs',$ltype->data['id']));

  }

$form->setDefaults($ltype->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>