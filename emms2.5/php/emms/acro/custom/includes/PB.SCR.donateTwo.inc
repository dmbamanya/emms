<?php
require_once 'class/loan_master.php';
require_once 'class/society.php';

$ids = LOAN_MASTER::publishable_ids();
if (!array_key_exists($_POST['master_id'], $ids)) 
{ 
  header(sprintf("Location: %s", WEBPAGE::$conf['mod_sponsorship']['group_sold_out']));
  exit;
}

$donation = donation_filter(str_replace(',','',$_POST['donation']),$_POST['tip']);
if (!$donation['ACK']) 
{ 
  header(sprintf("Location: %s", WEBPAGE::$conf['mod_sponsorship']['donation_filter_error']));
  exit; 
}

$master = new LOAN_MASTER($_POST['master_id']);
$master->loadBorrowerData();

$paypal_data = array
               (
                   'METHOD'                           => 'SetExpressCheckout',

                   'USER'                             => WEBPAGE::$conf['paypal']['USER'],
                   'PWD'                              => WEBPAGE::$conf['paypal']['PWD'],
                   'SIGNATURE'                        => WEBPAGE::$conf['paypal']['SIGNATURE'],

                   'VERSION'                          => WEBPAGE::$conf['paypal']['VERSION'],

                   'RETURNURL'                        => WEBPAGE::$conf['paypal']['RETURNURL'],
                   'CANCELURL'                        => WEBPAGE::$conf['paypal']['CANCELURL'],

		   'SOLUTIONTYPE'                     => 'Sole',
//		   'LANDINGPAGE'                      => 'Billing',

                   'PAYMENTREQUEST_0_PAYMENTACTION'   => WEBPAGE::$conf['paypal']['PAYMENTACTION'],
                   'PAYMENTREQUEST_0_CURRENCYCODE'    => WEBPAGE::$conf['paypal']['CURRENCYCODE'],
                   'PAYMENTREQUEST_0_AMT'             => $donation['TOTAL'],
		   'PAYMENTREQUEST_0_ITEMAMT'         => $donation['TOTAL'],

		   'L_PAYMENTREQUEST_0_ITEMCATEGORY0' => 'Digital',
		   'L_PAYMENTREQUEST_0_NAME0'         => WEBPAGE::$gt['donations_paypal_donation'],
		   'L_PAYMENTREQUEST_0_DESC0'         => strtoupper($master->data['borrower']->data['name']),
		   'L_PAYMENTREQUEST_0_QTY0'          => '1',
		   'L_PAYMENTREQUEST_0_AMT0'          => $donation['DONATION'],

                   'LOCALECODE'                       => WEBPAGE::$conf['paypal']['LOCALECODE'],
                   'HDRIMG'                           => WEBPAGE::$conf['paypal']['HDRIMG'],
                   'HDRBORDERCOLOR'                   => WEBPAGE::$conf['paypal']['HDRBORDERCOLOR'],
                   'HDRBACKCOLOR'                     => WEBPAGE::$conf['paypal']['HDRBACKCOLOR'],
                   'PAYFLOWCOLOR'                     => WEBPAGE::$conf['paypal']['PAYFLOWCOLOR'],

                   'REQCONFIRMSHIPPING'               => WEBPAGE::$conf['paypal']['REQCONFIRMSHIPPING'],
                   'NOSHIPPING'                       => WEBPAGE::$conf['paypal']['NOSHIPPING']
);

if($donation['TIP'] != '0.00')
{
    $paypal_data = $paypal_data + array
                (
		   'L_PAYMENTREQUEST_0_ITEMCATEGORY1' => 'Digital',
		   'L_PAYMENTREQUEST_0_NAME1'         => WEBPAGE::$gt['donations_paypal_tipname'],
		   'L_PAYMENTREQUEST_0_DESC1'         => WEBPAGE::$gt['donations_paypal_tipdesc'],
		   'L_PAYMENTREQUEST_0_QTY1'          => '1',
		   'L_PAYMENTREQUEST_0_AMT1'          => $donation['TIP']
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
 * Redirection to paypal login
 */

if($res['ACK'] == 'Success')
{
    $xrate = current(WEBPAGE::$dbh->getAssoc(sprintf
    (
     "select currency_id, rate from tblCurrenciesExchangeRates where date = curdate() and currency_id = '%s'",
        WEBPAGE::$conf['app']['xrate_local']))
    );
    
    if ($master->add_donation($xrate*$donation['DONATION'],$xrate*$donation['TIP'],$res['TOKEN']))
    {
      //$redirect_url = sprintf(WEBPAGE::$conf['paypal']['XCHECKOUTURL'],$res['TOKEN']);
      //print "Redirect: <a href='$redirect_url' target='_blank'>$redirect_url</a>";
      header(sprintf("Location: %s", sprintf(WEBPAGE::$conf['paypal']['XCHECKOUTURL'],$res['TOKEN'])));
      exit;
    } else {
      header(sprintf("Location: %s", WEBPAGE::$conf['mod_sponsorship']['group_sold_out']));
      exit;
    }
} else {

    header(sprintf("Location: %s", WEBPAGE::$conf['mod_sponsorship']['paypal_error']));
    exit;
}

function donation_filter($donation,$tip)
{
    if (!is_numeric($donation)) { return array('ACK' => false, 'ERROR' => 'IS_NUMERIC'); }
    if ($donation < WEBPAGE::$conf['mod_sponsorship']['min_donation']) { return array('ACK' => false, 'ERROR' => 'MIN_DONATION'); }
    $tip_amount = $tip ? $donation * (WEBPAGE::$conf['mod_sponsorship']['admin_tip'] / 100) : 0;
    return array
           (
            'ACK' => true,
            'DONATION' => number_format($donation, 2, '.', ''),
            'TIP'      => number_format($tip_amount, 2, '.', ''),
            'TOTAL'    => number_format($donation + $tip_amount, 2, '.', '')
           );
}

?>


<pre>Data sent: <?php print_r($paypal_data); ?></pre>
<pre>Result: <?php print_r($res); ?></pre>
<pre>cURL: <?php print_r($r); ?></pre>
<?php $tpl->setVariable('org_name',            WEBPAGE::$conf['app']['client_name']); ?>