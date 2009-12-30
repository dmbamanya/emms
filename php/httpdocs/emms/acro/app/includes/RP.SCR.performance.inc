<?php
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

$tpl->setVariable('fact',					WEBPAGE::$gt['fact']);
$tpl->setVariable('value',					WEBPAGE::$gt['value']);

$tpl->setVariable('writeOff_amount',		number_format($writeOff_amount, 2, '.', ','));
$tpl->setVariable('number_of_loans',		number_format($number_of_loans, 0, '.', ','));
$tpl->setVariable('number_of_business',		number_format($number_of_business, 0, '.', ','));
$tpl->setVariable('amount_disbursed',		number_format($amount_disbursed, 2, '.', ','));
$tpl->setVariable('active_associates',		number_format($active_associates, 0, '.', ','));
$tpl->setVariable('active_BDEs',			number_format($active_BDEs, 0, '.', ','));
$tpl->setVariable('active_groups',			number_format($active_groups, 0, '.', ','));
$tpl->setVariable('loan_amount_average',	number_format($loan_amount_average, 2, '.', ','));
$tpl->setVariable('portfolio_balance',		number_format($portfolio_balance, 2, '.', ','));
$tpl->setVariable('payment_rate',			$payment_rate);
$tpl->setVariable('women_rate',				$women_rate);
$tpl->setVariable('amount_repaid',			number_format($amount_repaid, 2, '.', ','));
$tpl->setVariable('advisors',				number_format($advisors, 0, '.', ','));
$tpl->setVariable('administrative_staff',	number_format($advisors+$administrative_staff, 0, '.', ','));
$tpl->setVariable('retention_rate',			$retention_rate);

$tpl->setVariable('label_writeOff_amount',		WEBPAGE::$gt['writeOff_amount']);
$tpl->setVariable('label_number_of_loans',		WEBPAGE::$gt['number_of_loans']);
$tpl->setVariable('label_number_of_business',	WEBPAGE::$gt['number_of_business']);
$tpl->setVariable('label_amount_disbursed',		WEBPAGE::$gt['amount_disbursed']);
$tpl->setVariable('label_active_associates',	WEBPAGE::$gt['active_associates']);
$tpl->setVariable('label_active_BDEs',			WEBPAGE::$gt['active_BDEs']);
$tpl->setVariable('label_active_groups',		WEBPAGE::$gt['active_groups']);
$tpl->setVariable('label_loan_amount_average',	WEBPAGE::$gt['loan_amount_average']);
$tpl->setVariable('label_portfolio_balance',	WEBPAGE::$gt['portfolio_balance']);
$tpl->setVariable('label_payment_rate',			WEBPAGE::$gt['payment_rate']);
$tpl->setVariable('label_women_rate',			WEBPAGE::$gt['women_rate']);
$tpl->setVariable('label_amount_repaid',		WEBPAGE::$gt['amount_repaid']);
$tpl->setVariable('label_advisors',				WEBPAGE::$gt['advisors']);
$tpl->setVariable('label_administrative_staff',	WEBPAGE::$gt['administrative_staff']);
$tpl->setVariable('label_retention_rate',		WEBPAGE::$gt['retention_rate']);

?>