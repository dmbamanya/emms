<?php
WEBPAGE::$dbh->query(sprintf("delete from tblLoansMasterSponsors where token = '%s' and (unix_timestamp('%s') - unix_timestamp(datetime) < 60 * %s)", $_GET['token'],gmdate("Y-m-d H:i:s", time()),WEBPAGE::$conf['mod_sponsorship']['donation_entry_delay']));
header(sprintf("Location: %s", WEBPAGE::$conf['mod_sponsorship']['urlsite']));
exit;
?>