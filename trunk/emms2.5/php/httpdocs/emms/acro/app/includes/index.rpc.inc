<?php
require_once 'XML/RPC2/Server.php';

class RpcSrv
{
     /**
     * Return array with performance stats
     *
     * @param none
     * @return array Performance stats
     */
    public static function performance()
    {
        $writeOff_amount 		 = WEBPAGE::$conf['app']['ini.writeOff_amount'];
        $number_of_loans 		 = WEBPAGE::$conf['app']['ini.number_of_loans'];
        $number_of_business 	         = WEBPAGE::$conf['app']['ini.number_of_business'];
        $amount_disbursed 		 = WEBPAGE::$conf['app']['ini.amount_disbursed'];
        $writeOff_amount 		+= current(current(WEBPAGE::$dbh->getAll("SELECT SUM(amount) FROM tblLoanWriteOff")));
        $number_of_loans 		+= current(current(WEBPAGE::$dbh->getAll("SELECT COUNT(id) FROM tblLoanStatusHistory WHERE status = 'G'")));
        $number_of_business 	        += current(current(WEBPAGE::$dbh->getAll("SELECT COUNT(id) FROM tblBusiness")));
        $amount_disbursed 		+= current(current(WEBPAGE::$dbh->getAll("SELECT SUM(l.kp) FROM tblLoans AS l, tblLoanStatusHistory AS lsh WHERE lsh.loan_id = l.id AND lsh.status = 'G'")));
        $active_associates 		 = current(current(WEBPAGE::$dbh->getAll("SELECT COUNT(id) FROM tblClients WHERE advisor_id > 0 AND zone_id > 0")));
        $active_BDEs 			 = current(current(WEBPAGE::$dbh->getAll("SELECT COUNT(id) FROM tblSocieties WHERE advisor_id > 0 AND zone_id > 0 AND category = 'BG'")));
        $active_groups 			 = current(current(WEBPAGE::$dbh->getAll("SELECT COUNT(id) FROM tblSocieties WHERE advisor_id > 0 AND zone_id > 0 AND category = 'B'")));
        $loan_amount_average 	         = $number_of_loans ? $amount_disbursed / $number_of_loans : 0;
        $portfolio_balance 		 = round(current(current(WEBPAGE::$dbh->getAll("SELECT SUM(balance_kp) FROM tblLoansCurrentData"))));
        $payment_rate 			 = ($amount_disbursed-$portfolio_balance) ? round(100 * (1 - ($writeOff_amount/($amount_disbursed-$portfolio_balance))),2) : 0;
        $women_rate 			 = $active_associates ? round((100 * current(current(WEBPAGE::$dbh->getAll("SELECT COUNT(id) FROM tblClients WHERE advisor_id > 0 AND zone_id > 0 AND gender = 'F'"))) / $active_associates),2) : 0;
        $amount_repaid 			 = $amount_disbursed - $writeOff_amount - $portfolio_balance;
        $advisors 		         = (WEBPAGE::$conf['app']['performance_staff_advisors']==0) ? count(WEBPAGE::$dbh->getAll("SELECT COUNT(advisor_id) AS advisors FROM tblClients where zone_id > 0 AND advisor_id > 0 GROUP BY advisor_id")) : WEBPAGE::$conf['app']['performance_staff_advisors'];
        $administrative_staff            = (WEBPAGE::$conf['app']['performance_staff_administrative']==0) ? current(current(WEBPAGE::$dbh->getAll("SELECT COUNT(id) FROM tblUsers WHERE active = 1 and staff = 1"))) - $advisors : WEBPAGE::$conf['app']['performance_staff_administrative'];
        $attrition 		         = current(current(WEBPAGE::$dbh->getAll("SELECT COUNT(id) FROM tblClients WHERE zone_id = 0 OR advisor_id = 0")));
        $retention_rate 		 = ($attrition + $active_associates) ? round(100*(1-($attrition/($attrition + $active_associates))),2) : 0;

        $stats = array
                 (
                    'writeOff_amount'      => $writeOff_amount,
                    'number_of_loans'      => $number_of_loans,
                    'number_of_business'   => $number_of_business,
                    'amount_disbursed'     => $amount_disbursed,
                    'active_associates'    => $active_associates,
                    'active_BDEs'          => $active_BDEs,
                    'active_groups'        => $active_groups,
                    'loan_amount_average'  => $loan_amount_average,
                    'portfolio_balance'    => $portfolio_balance,
                    'payment_rate'         => $payment_rate,
                    'women_rate'           => $women_rate,
                    'amount_repaid'        => $amount_repaid,
                    'advisors'             => $advisors,
                    'administrative_staff' => $administrative_staff,
                    'attrition'            => $attrition,
                    'retention_rate'       => $retention_rate,
                    'usd_xrate'            => WEBPAGE::$conf['app']['usd_xrate']
                 );
        return $stats;
    }

}

$options = array
           (
                'prefix' => 'emms.' // define a sort of "namespace" for the server
           );

$server = XML_RPC2_Server::create('RpcSrv', $options);
$server->handleCall();
?>
