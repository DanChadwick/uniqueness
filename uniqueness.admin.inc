<?php
// $Id$

/**
 * @file
 * Settings page for the uniqueness module.
 */

/**
 * Form builder for uniqueness settings page; system_settings_form().
 */
function uniqueness_settings() {
  $form = array();

  // Appearance.
  $form['appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['appearance']['uniqueness_widgets'] = array(
    '#type' => 'checkboxes',
    '#options' => array(
      UNIQUENESS_WIDGET_INLINE => t('Display related content inline, embedded on the "node add" form.'),
      UNIQUENESS_WIDGET_BLOCK => t('Provide a block for displaying related content.'),
    ),
    '#default_value' => variable_get('uniqueness_widgets', array(UNIQUENESS_WIDGET_INLINE)),
  );
  $form['appearance']['uniqueness_default_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Default title'),
    '#default_value' => variable_get('uniqueness_default_title', t('Related content')),
    '#description' => t('Note: when using the widget as a block then this title can be overriden by the block title on the !block_settings_page.',
        array('!block_settings_page' => l(t('uniqueness block settings page'), 'admin/build/block/configure/uniqueness/uniqueness'))),
    '#required' => TRUE,
  );
  $form['appearance']['uniqueness_default_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Default description'),
    '#default_value' => variable_get('uniqueness_default_description',
        t("Help us increase the signal to noise ratio! If we find content that's related or similar to what you're posting it will be listed here.")),
    '#rows' => 2,
  );
  $form['appearance']['uniqueness_results_prepend'] = array(
    '#type' => 'radios',
    '#title' => t('Results display'),
    '#options' => array(
      0 => t('Replace old results with new ones'),
      1 => t('Leave old results and prepend new ones'),
    ),
    '#default_value' => variable_get('uniqueness_results_prepend', 0),
    '#description' => t('Choose if new results replace or are added to existing results. Browser cache may keep this setting from taking affect right away.'),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}
