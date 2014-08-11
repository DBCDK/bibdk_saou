<?php dpm($variables);?>
<div class="dropdown-wrapper">
<a class="dropdown-toggle" href="#"><?php print $link['text']; ?></a>
<?php if(!empty($link['message'])) :?>
<div class="visuallyhidden dropdown-menu btn">
    <?php echo $link['message'];?>
</div>
<?php endif;?>
</div>
