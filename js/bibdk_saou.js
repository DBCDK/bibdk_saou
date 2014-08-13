(function ($) {

  Drupal.bibdkHandleSaou = function(saou){
    var link = $('#bibdk_saou_'+saou.selector);
    if(saou.error.length > 0){
      // remove throbber
      link.find('span').remove();
      link.addClass('dropdown-toggle');
      link.after(saou.error);
    }
    else{
      link.replaceWith(saou.redirect);
    }
  }

  Drupal.bibdkGetSaouRessource = function(ressource){
    var link = ressource;
    link.append('<span class="ajax-progress"><span class="throbber"></span></span>');
    var pid = link.attr('data-pid');
    var url = link.attr('data-url');
    var destination = link.attr('href');

    var request = $.ajax({
      url:destination,
      type: 'POST',
      data: {
        pid: pid
      },
      dataType: 'json',
      success: Drupal.bibdkHandleSaou
    });
  }

  Drupal.behaviors.bibdk_saou_load = {
    attach: function (context) {
      $('.soau-ressource-link', context).click(function (e) {
        e.preventDefault();
        Drupal.bibdkGetSaouRessource($(this));
      });
    }
  };
}
      (jQuery));