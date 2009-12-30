#!/usr/bin/php
<?php

$path = '/base_path_to_conf.ini_file/emms/acro';
$conf = parse_ini_file($path . '/conf.ini', true);

$cmd = sprintf("gunzip -c %s/backup/initialbk.gz | mysql -u %s --password=%s %s",$path,$conf['db']['usr'],$conf['db']['pwd'],$conf['db']['name']);
shell_exec($cmd);

?>