// $Id$

Drupal.behaviors.uniqueness = function (context) {
  uniqueness = new Drupal.uniqueness(Drupal.settings.uniqueness['URL'], $('.uniqueness-dyn'));
  if (Drupal.settings.uniqueness['preview'] == true) {
    uniqueness.clear();
  }
  // Search off title.
  $('#edit-title').keyup(function() {
    input = this.value;
    if (input.length > 0) {
      uniqueness.search('title', input);
    }
    else if(input.length == 0 && !uniqueness.prependResults) {
      uniqueness.clear();
    }
  });
  // Search off tags.
  $('#edit-taxonomy-tags-1').blur(function() {
    input = this.value;
    // Some tags set.
    if (input.length > 0) {
      uniqueness.search('tags', input);
    }
  });
};

Drupal.uniqueness = function (uri, widget) {
  this.uri = uri;
  this.delay = 500;
  this.widget = widget;
  this.widget.append('<span class="uniqueness-dyn-span"></span').append('<ul class="uniqueness-dyn-ul"></ul>');
  this.list = $('ul.uniqueness-dyn-ul');
  this.notifier = $('span.uniqueness-dyn-span');
  this.widgetCSS = {
    'background-image' : "url('" + Drupal.settings.basePath + "misc/throbber.gif" + "')",
    'background-position' : '100% -18px',
    'background-repeat' : 'no-repeat'
  }
  this.searchCache = {};
  this.listCache = {};
  this.prependResults = Drupal.settings.uniqueness['prependResults'];
}

Drupal.uniqueness.prototype.update = function (data) {
  uniqueness.notifier.removeClass('uniqueness-dyn-searching').empty();
  uniqueness.widget.css('background-image', '');
  uniqueness = this;
  if (uniqueness.prependResults) {
    if (data == undefined && uniqueness.listCache != {}) {
      data = uniqueness.listCache;
    }
    var items = '';
    $.each(data, function(i, item) {
      // Only use what we haven't seen before.
      if (uniqueness.listCache[item.nid] == undefined) {
        items += '<li><a href="' + item.href + '" target="_blank">' + item.title + '</a></li>';
        // Store the new item.
        uniqueness.listCache[item.nid] = item;
      }
    });
    // Show list.
    this.list.prepend(items);
  }
  else { // Replace content. //@todo still use caching?
    if (data == undefined) {
      uniqueness.clear();
      return;
    }
    var items = '';
    $.each(data, function(i, item) {
      items += '<li><a href="' + item.href + '" target="_blank">' + item.title + '</a></li>';
    });
    this.list.html(items);
  }
}

Drupal.uniqueness.prototype.search = function (element, searchString) {
  uniqueness = this;

  // If this string has been searched for before we do nothing.
  if (uniqueness.prependResults && uniqueness.searchCache[searchString]) {
    return;
  }

  if (this.timer) {
    clearTimeout(this.timer);
  }
  this.timer = setTimeout(function () {
    // Inform user we're searching.
    if (uniqueness.notifier.hasClass('uniqueness-dyn-searching') == false) {
      uniqueness.notifier.addClass('uniqueness-dyn-searching').append('Searching ...');
      uniqueness.widget.css(uniqueness.widgetCSS);
    }
    $.getJSON(uniqueness.uri + '?' + element + '=' + searchString, function (data) {
      if (data != undefined && data != 'false') {
        // Found results.
        uniqueness.update(data);
        // Save this string, it found results.
        uniqueness.searchCache[searchString] = searchString;
        var blockSet = true;
      }
      // Nothing new found so show existing results.
      if (blockSet == undefined) {
        uniqueness.update();
      }
    });
  }, uniqueness.delay);
}

Drupal.uniqueness.prototype.clear = function () {
  this.list.empty();
}