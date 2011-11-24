#!/usr/bin/php
<?php

$path = '/var/www/vhosts/esperanza.org/emms/debug';
$conf = parse_ini_file($path . '/conf.ini', true);

$linkID = @mysql_connect($conf['db']['host'],$conf['db']['usr'],$conf['db']['pwd']) or die("Could not connect to MySQL server");
@mysql_select_db($conf['db']['name']) or die("Could not select db");
mysql_query(sprintf("delete from tblLoansMasterSponsors where sponsor_id is null and (unix_timestamp('%s') - unix_timestamp(datetime) > 60 * %s)", gmdate("Y-m-d H:i:s", time()),$conf['mod_sponsorship']['donation_entry_delay']));
mysql_close();

?>