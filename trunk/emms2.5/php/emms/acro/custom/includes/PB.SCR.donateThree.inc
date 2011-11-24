<?php
require_once 'class/loan_master.php';
require_once 'class/society.php';
require_once '../mod_sponsorship/class/sponsor.php';

$paypal_data = array
               (
                   'TOKEN'                          => $_GET['token'],
                   'METHOD'                         => 'GetExpressCheckoutDetails',
                   'USER'                           => WEBPAGE::$conf['paypal']['USER'],
                   'PWD'                            => WEBPAGE::$conf['paypal']['PWD'],
                   'SIGNATURE'                      => WEBPAGE::$conf['paypal']['SIGNATURE'],
                   'VERSION'                        => WEBPAGE::$conf['paypal']['VERSION']
               );

$ch = curl_init(); // Start cURL
curl_setopt_array( $ch, array
                        (   // SetParameters
                            CURLOPT_URL		   => WEBPAGE::$conf['paypal']['PAYPALURL'],
                            CURLOPT_SSL_VERIFYPEER => FALSE,
                            CURLOPT_SSL_VERIFYHOST => FALSE,
                            CURLOPT_RETURNTRANSFER => 1,
                            CURLOPT_TIMEOUT	   => 60,
                            CURLOPT_POST	   => 1,
                            CURLOPT_POSTFIELDS     => http_build_query(array_filter($paypal_data))
                        ));

$r = curl_exec($ch); // get page from PayPal

if(curl_error($ch))
{
    $r .= "cURL ERROR: ".curl_errno($ch).": ".curl_error($ch);
    }
        else
    {
    // extract the tokens
    $res = array();
    preg_match_all('/([a-z0-9\_]+)=([^&]+)/i',$r, $matches, PREG_SET_ORDER);
    foreach($matches as $o)
    {
	$res[$o[1]] = urldecode($o[2]);
    }
}
curl_close($ch);

/*
 * Something went wrong with GetExpressCheckoutDetails
 */
if($res['ACK'] != 'Success')
{
    header(sprintf("Location: %s", WEBPAGE::$conf['mod_sponsorship']['paypal_error']));
    exit;
}

$res1 = $res;

/*
 * Retrive pre-registered donation info
 * and current xrate
 */
$pdonation = current(WEBPAGE::$dbh->getAll(sprintf("select * from tblLoansMasterSponsors where token = '%s'", $res['TOKEN'])));
$xrate = current(WEBPAGE::$dbh->getAssoc(sprintf
    (
     "select currency_id, rate from tblCurrenciesExchangeRates where date = curdate() and currency_id = '%s'",
        WEBPAGE::$conf['app']['xrate_local']))
    );
$master = new LOAN_MASTER($pdonation['master_id']);
$master->loadBorrowerData();

/*
 * Everything is ok with GetExpressCheckoutDetails
 * Proceed with DoExpressCheckoutPayment
 */

$paypal_data = array
               (
                   'TOKEN'                          => $res['TOKEN'],
                   'PAYERID'                        => $res['PAYERID'],
                   'METHOD'                         => 'DoExpressCheckoutPayment',
                   'USER'                           => WEBPAGE::$conf['paypal']['USER'],
                   'PWD'                            => WEBPAGE::$conf['paypal']['PWD'],
                   'SIGNATURE'                      => WEBPAGE::$conf['paypal']['SIGNATURE'],
                   'VERSION'                        => WEBPAGE::$conf['paypal']['VERSION'],

                   'PAYMENTREQUEST_0_PAYMENTACTION'   => WEBPAGE::$conf['paypal']['PAYMENTACTION'],
                   'PAYMENTREQUEST_0_CURRENCYCODE'    => WEBPAGE::$conf['paypal']['CURRENCYCODE'],
                   'PAYMENTREQUEST_0_AMT'             => number_format(($pdonation['donation'] + $pdonation['tip'])/$xrate, 2, '.', ''),
		   'PAYMENTREQUEST_0_ITEMAMT'         => number_format(($pdonation['donation'] + $pdonation['tip'])/$xrate, 2, '.', ''),

		   'L_PAYMENTREQUEST_0_ITEMCATEGORY0' => 'Digital',
		   'L_PAYMENTREQUEST_0_NAME0'         => WEBPAGE::$gt['donations_paypal_donation'],
		   'L_PAYMENTREQUEST_0_DESC0'         => strtoupper($master->data['borrower']->data['name']),
		   'L_PAYMENTREQUEST_0_QTY0'          => '1',
		   'L_PAYMENTREQUEST_0_AMT0'          => number_format($pdonation['donation']/$xrate, 2, '.', '')
               );

if($donation['TIP'] != '0.00')
{
    $paypal_data = $paypal_data + array
                (
		   'L_PAYMENTREQUEST_0_ITEMCATEGORY1' => 'Digital',
		   'L_PAYMENTREQUEST_0_NAME1'         => WEBPAGE::$gt['donations_paypal_tipname'],
		   'L_PAYMENTREQUEST_0_DESC1'         => WEBPAGE::$gt['donations_paypal_tipdesc'],
		   'L_PAYMENTREQUEST_0_QTY1'          => '1',
		   'L_PAYMENTREQUEST_0_AMT1'          => number_format($pdonation['tip']/$xrate, 2, '.', '')
                );
}

$ch = curl_init(); // Start cURL
curl_setopt_array( $ch, array
                        (   // SetParameters
                            CURLOPT_URL		   => WEBPAGE::$conf['paypal']['PAYPALURL'],
                            CURLOPT_SSL_VERIFYPEER => FALSE,
                            CURLOPT_SSL_VERIFYHOST => FALSE,
                            CURLOPT_RETURNTRANSFER => 1,
                            CURLOPT_TIMEOUT	   => 60,
                            CURLOPT_POST	   => 1,
                            CURLOPT_POSTFIELDS     => http_build_query(array_filter($paypal_data))
                        ));

$r = curl_exec($ch); // get page from PayPal

if(curl_error($ch))
{
    $r .= "cURL ERROR: ".curl_errno($ch).": ".curl_error($ch);
    }
        else
    {
    // extract the tokens
    $res = array();
    preg_match_all('/([a-z0-9\_]+)=([^&]+)/i',$r, $matches, PREG_SET_ORDER);
    foreach($matches as $o)
    {
	$res[$o[1]] = urldecode($o[2]);
    }
}
curl_close($ch);

/*
 * Something went wrong with DoExpressCheckoutPayment
 */
if($res['ACK'] != 'Success')
{
    header(sprintf("Location: %s", WEBPAGE::$conf['mod_sponsorship']['paypal_error']));
    exit;
}

/*
$verified_sponsor = false;
if ($res1['PAYERSTATUS'] == 'verified')
{
  $verified_sponsor = true;
}
*/

$sponsor_id = current(WEBPAGE::$dbh->getAssoc(sprintf("select id,id from tblSponsors where username = '%s'", $res1['EMAIL'])));
$sponsor = new SPONSOR($sponsor_id);

$new_sponsor = false;
if(!is_numeric($sponsor_id))
{
  $new_sponsor                   = true;
  $new_sponsor_pwd               = getRandomCode(6);

  $sponsor->data['username']     = $res1['EMAIL']; // $verified_sponsor ? $res1['EMAIL'] : '';
  $sponsor->data['password']     = crypt($new_sponsor_pwd, md5($new_sponsor_pwd));
  $sponsor->data['sponsor']      = sprintf(WEBPAGE::$conf['mod_sponsorship']['boh_title'],$res1['FIRSTNAME'],$res1['LASTNAME']);
  $sponsor->data['first']        = $res1['FIRSTNAME'];
  $sponsor->data['middle']       = '';
  $sponsor->data['last']         = $res1['LASTNAME'];
  $sponsor->data['email']        = $res1['EMAIL'];
  $sponsor->data['memo']         = '';
  $sponsor->data['creator_id']   = 1;
  $sponsor->data['creator_date'] = date('Y-m-d');
  $sponsor->data['editor_id']    = 1;
  $sponsor->data['editor_date']  = date('Y-m-d');
  $sponsor->data['active']       = '1'; // $verified_sponsor ? '1' : '0';
//  $sponsor->data['actcode']      = $verified_sponsor ? '' : getRandomCode(20);

  $sponsor_id = $sponsor->add();
}

/*
 * Register the new donation in tblSponsorsDonations and tblLoansMasterSponsors
 */
WEBPAGE::$dbh->query(sprintf("insert into tblSponsorsDonations
                                              (sponsor_id,src_amount,src_tip,src_currency_id,conv_amount,conv_tip,conv_currency_id,creator_id,creator_date,editor_id,editor_date,token,memo)
                                           values
                                              ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
                                              $sponsor_id,
                                              $pdonation['donation']/$xrate,$pdonation['tip']/$xrate,WEBPAGE::$conf['app']['xrate_reference'],
                                              $pdonation['donation'],$pdonation['tip'],WEBPAGE::$conf['app']['xrate_local'],
                                              1,date('Y-m-d'),1,date('Y-m-d'),$res['TOKEN'],$res1['PAYMENTREQUEST_0_NOTETEXT']));
$master->commit_donation($res['TOKEN'],$sponsor_id);

// insert code here to redirect non active accounts
/*
if((!$verified_sponsor)&&($new_sponsor))
{

}
*/
    
$tpl->setVariable('org_name',                 WEBPAGE::$conf['app']['client_name']);
$tpl->setVariable('donations_program_title',  WEBPAGE::$conf['mod_sponsorship']['donations_title']);
$tpl->setVariable('donations_thankyou',       WEBPAGE::$gt['donations_thankyou']);
$tpl->setVariable('donations_dear',           sprintf(WEBPAGE::$gt['donations_dear'],$res1['FIRSTNAME'].' '.$res1['LASTNAME']));
$tpl->setVariable('donations_tax',            sprintf(WEBPAGE::$gt['donations_tax'],WEBPAGE::$conf['app']['client_name']));

if ($new_sponsor == true)
{
  $tpl->setVariable('donations_msg', sprintf(WEBPAGE::$gt['donations_new'],$sponsor->data['sponsor'],$sponsor->data['username'],$new_sponsor_pwd,WEBPAGE::$conf['mod_sponsorship']['urlembedded'],WEBPAGE::$conf['mod_sponsorship']['urlembedded'])); 
  $tpl->setVariable('donations_thankyou_again', sprintf(WEBPAGE::$gt['donations_thankyou_again_new'],WEBPAGE::$conf['app']['client_name']));
} else {
  $tpl->setVariable('donations_msg', sprintf(WEBPAGE::$gt['donations_returning'],$sponsor->data['sponsor'],WEBPAGE::$conf['mod_sponsorship']['urlembedded']));
  $tpl->setVariable('donations_thankyou_again', sprintf(WEBPAGE::$gt['donations_thankyou_again_returning'],WEBPAGE::$conf['app']['client_name']));
}

function getRandomCode($length)
{
  for($code_length=$length,$newcode='';strlen($newcode)<$code_length;$newcode.=chr(!rand(0,2)?rand(48,57):(!rand(0,1)?rand(65,90):rand(97,122))));
  return $newcode;
}

/*
    <pre>Payment completed:</pre>
    <pre>Data sent: <?php print_r($paypal_data); ?></pre>
    <pre>Result GetDetails: <?php print_r($res1); ?></pre>
    <pre>Result DoPayment: <?php print_r($res); ?></pre>
*/
?>