<?php
if (!$_REQUEST['sponsor_id']) {
  WEBPAGE::redirect('index.php?scr_name=SP.SCR.browseSponsors&ref=SP.SCR.addDonation');
  }

require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/fund.php';
require_once '../mod_sponsorship/class/sponsor.php';

$fund = new FUND();
$sponsor = new SPONSOR($_REQUEST['sponsor_id']);
$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('fundForm','POST');

$form->addElement('static', 	'submit', 	'',     $btn->imgsubmit('fundForm',WEBPAGE::$gt['submit']));
$form->addElement('static', 	'reset', 	  '',     $btn->imgreset('fundForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 	'cancel', 	'',     $btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));

$form->addElement('static', 	'sponsor', 	        WEBPAGE::$gt['tblSponsors.sponsor'],     $sponsor->data['sponsor']);

$form->addElement('hidden',		'scr_name',		      $_REQUEST['scr_name']);
$form->addElement('hidden',		'id',			          $_REQUEST['id']);
$form->addElement('hidden',		'sponsor_id',			  $_REQUEST['sponsor_id']);

$form->addElement('text',		  'src_amount',			  WEBPAGE::$gt['tblSponsorsDonations.src_amount'], 'class=large');
$form->addElement('text',		  'conv_amount',			WEBPAGE::$gt['localCurrencyAmountEq'],           'class=large');

$form->addElement('select',		'src_currency_id',	WEBPAGE::$gt['tblSponsorsDonations.src_currency_id'],	  $fund->currencys(), 'class=large');
$form->addElement('select',		'conv_currency_id',	WEBPAGE::$gt['tblSponsorsDonations.conv_currency_id'],	$fund->currencys(), 'class=large');

//add rules
$form->addRule('src_amount',				  WEBPAGE::$gt['required'],		       'required');
$form->addRule('src_amount',				  WEBPAGE::$gt['nonValidEntry'],		 'numeric');
$form->addRule('conv_amount',				  WEBPAGE::$gt['required'],		       'required');
$form->addRule('conv_amount',				  WEBPAGE::$gt['nonValidEntry'],		 'numeric');
$form->addRule('src_currency_id',		  WEBPAGE::$gt['selectOption'],      'required');
$form->addRule('src_currency_id',		  WEBPAGE::$gt['selectOption'],      'alphanumeric');
$form->addRule('conv_currency_id',		WEBPAGE::$gt['selectOption'],      'required');
$form->addRule('conv_currency_id',		WEBPAGE::$gt['selectOption'],      'alphanumeric');

//$form->freeze($fund->frostdata);

if ($form->validate()) {

  $form->freeze();

  $values = $form->_submitValues;
  
  WEBPAGE::$dbh->query(sprintf("insert into tblSponsorsDonations
                                              (sponsor_id,src_amount,src_currency_id,conv_amount,conv_currency_id,creator_id,creator_date,editor_id,editor_date)
                                           values
                                              ('%s','%s','%s','%s','%s','%s','%s','%s','%s')",
                                              $values['sponsor_id'],
                                              $values['src_amount'],$values['src_currency_id'],
                                              $values['conv_amount'],$values['conv_currency_id'],
                                              WEBPAGE::$userID,date('Y-m-d'),WEBPAGE::$userID,date('Y-m-d')));

//  $values['editor_date']  = date('Y-m-d');
//  $values['creator_date'] = $fund->funddata['creator_date'] ? $fund->funddata['creator_date'] : $values['editor_date'];
//  $values['editor_id']    = WEBPAGE::$userID;
//  $values['creator_id']   = $fund->funddata['creator_id']   ? $fund->funddata['creator_id']   : $values['editor_id'];

//  $lastID = SQL::insert('tblFunds',$values,'id');

  WEBPAGE::redirect(sprintf('index.php?scr_name=SP.SCR.donationHistoryBySponsor&ref=SP.SCR.addDonation&id=%s&ico=scs&msg=scs',$values['sponsor_id']));

  }

//$form->setDefaults($fund->funddata);

$tpl->setVariable('donation_link_href', sprintf('javascript:openWin("index.popup.php?scr_name=SP.SCR.donationHistoryBySponsor&id=%s","donationHistoryBySponsor","menubar=no,scrollbars=yes,resizable=yes,width=700,height=320")',$sponsor->data['id']));
$tpl->setVariable('donation_link_text', sprintf("%s. %s.",WEBPAGE::$gt['SP.SCR.donationsHistory'],WEBPAGE::$gt['clic2open']));

$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

?>